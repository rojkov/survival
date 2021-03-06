#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <string>
#include <vector>
#include "worldposition.h"
#include "world.h"
#include "terrain.h"
#include "tile.h"
#include "lifeform.h"
#include "viewport.h"
#include "graphalg/a_star_search.h"

static uint32_t g_last_ticks = 0;
static int g_fps = 0;

using unique_surf = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

inline int heuristic(GridLocation a, GridLocation b) {
    int x1, y1, x2, y2;
    std::tie(x1, y1) = a;
    std::tie(x2, y2) = b;
    return abs(x1 - x2) + abs(y1 + y2);
}

World::World(std::shared_ptr<SDL_Renderer> renderer)
    : m_renderer(renderer)
    , m_viewport(std::make_shared<Viewport>(WorldRect(0, 0, 640, 480)))
    , m_grass_terrain(nullptr)
    , m_water_terrain(nullptr)
    , m_texture(nullptr, SDL_DestroyTexture)
    , m_txt_rect(0, 0, 640 + TILE_WIDTH*4, 480 + TILE_HEIGHT*4)
    , m_selection_rect(0, 0, 0, 0)
    , m_mouse_down(false)
{
    unique_surf temp_surf(IMG_Load("tileset.png"), SDL_FreeSurface);
    assert(temp_surf != nullptr);
    unique_surf grass_surf(SDL_CreateRGBSurface(0, TILE_WIDTH, TILE_HEIGHT,
                                                32, 0, 0, 0, 0),
                           SDL_FreeSurface);
    assert(grass_surf != nullptr);
    unique_surf water_surf(SDL_CreateRGBSurface(0, TILE_WIDTH, TILE_HEIGHT,
                                                32, 0, 0, 0, 0),
                           SDL_FreeSurface);
    assert(water_surf != nullptr);

    SDL_Rect rect {0, 0, TILE_WIDTH, TILE_HEIGHT};
    SDL_BlitSurface(temp_surf.get(), &rect, grass_surf.get(), nullptr);
    rect.x = TILE_WIDTH * 1;
    SDL_BlitSurface(temp_surf.get(), &rect, water_surf.get(), nullptr);

    m_grass_terrain.reset(new Terrain(Terrain::GRASS, SDL_CreateTextureFromSurface(m_renderer.get(), grass_surf.get())));
    m_water_terrain.reset(new Terrain(Terrain::WATER, SDL_CreateTextureFromSurface(m_renderer.get(), water_surf.get())));

    m_tiles.load("world.map", [this](std::string token) -> std::unique_ptr<Tile> {
            switch (std::stoi(token)) {
            case Terrain::GRASS: {
                std::unique_ptr<Tile> grass_tile(new Tile(m_grass_terrain.get()));
                return grass_tile;
            }
            case Terrain::WATER: {
                std::unique_ptr<Tile> water_tile(new Tile(m_water_terrain.get()));
                return water_tile;
            }
            default:
                assert(false);
            }
    });

    // Create world texture
    m_texture.reset(SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      m_txt_rect.width,
                                      m_txt_rect.height));
    assert(m_texture != nullptr);
    refresh_texture();
}

World::~World() {}

std::vector<WorldPoint> World::get_path(const WorldPosition &start, const WorldPosition &end) const
{
    const auto current(location(start));
    const auto goal(location(end));

    int current_x, current_y, goal_x, goal_y;
    std::tie(current_x, current_y) = current;
    std::tie(goal_x, goal_y) = goal;
    if (m_tiles.at(current_x, current_y)->region() ==
            m_tiles.at(goal_x, goal_y)->region()) {
        std::function<int(GridLocation, GridLocation)> h_func = heuristic;
        auto path = a_star_search(m_tiles, current, goal, h_func);
        auto wpath = as_world_path(path);
        return wpath;
    } else {
        std::vector<WorldPoint> empty_path;
        return empty_path;
    }
}

GridLocation World::location(const WorldPosition& pos) const
{
    GridLocation location {(int)round(pos.x)/TILE_WIDTH,
                           (int)round(pos.y)/TILE_HEIGHT};
    return location;
}

GridLocation World::closest(const GridLocation& loc, const std::unordered_set<GridLocation>& locs) const
{
    return m_tiles.closest(loc, locs);
}

const WorldRect World::get_viewport() const
{
    return m_viewport->get_rect();
}

SDL_Rect World::to_sdl_rect(const WorldRect& rect) const
{
    return m_viewport->to_screen_rect(rect).as_sdl_rect();
}

void World::add_entity(std::shared_ptr<LifeForm> entity)
{
    m_lifeforms.push_back(entity);
}

void World::handle_event(const SDL_Event &event)
{
    const uint8_t* current_key_states = SDL_GetKeyboardState(nullptr);
    if (current_key_states[SDL_SCANCODE_UP]) {
        m_viewport->move(WorldPoint(0, -1));
    } else if (current_key_states[SDL_SCANCODE_DOWN]) {
        m_viewport->move(WorldPoint(0, 1));
    } else if (current_key_states[SDL_SCANCODE_LEFT]) {
        m_viewport->move(WorldPoint(-1, 0));
    } else if (current_key_states[SDL_SCANCODE_RIGHT]) {
        m_viewport->move(WorldPoint(1, 0));
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            WorldRect vrect = m_viewport->get_rect();
            m_selection_rect.x = event.button.x + vrect.x;
            m_selection_rect.y = event.button.y + vrect.y;
            m_mouse_down = true;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            // Calculate tile set to patrol by lifeforms
            if (m_selection_rect.width > 0 && m_selection_rect.height > 0) {
                std::unordered_set<GridLocation> patrol_set;
                for (int x = m_selection_rect.x / TILE_WIDTH;
                        x <= (m_selection_rect.x + m_selection_rect.width) / TILE_WIDTH;
                        x++) {
                    for (int y = m_selection_rect.y / TILE_HEIGHT;
                            y <= (m_selection_rect.y + m_selection_rect.height) / TILE_HEIGHT;
                            y++) {
                        GridLocation loc {x, y};
                        patrol_set.insert(loc);
                    }
                }
                for (auto entity : m_lifeforms) {
                    int x, y;
                    std::tie(x, y) = location(entity->get_pos());
                    auto reg = m_tiles.at(x, y)->region();
                    std::unordered_set<GridLocation> pset;
                    for (auto loc : patrol_set) {
                        int x, y;
                        std::tie(x, y) = loc;
                        if (m_tiles.at(x, y)->region() == reg) {
                            pset.insert(loc);
                        }
                    }
                    if (!pset.empty()) {
                        entity->patrol(pset);
                    }
                }
            }

            // Reset selection state
            m_selection_rect.width = 0;
            m_selection_rect.height = 0;
            m_mouse_down = false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        if (m_mouse_down) {
            WorldRect vrect = m_viewport->get_rect();
            int diff_x(event.motion.x + vrect.x - m_selection_rect.x);
            int diff_y(event.motion.y + vrect.y - m_selection_rect.y);
            if (diff_x > 0) {
                m_selection_rect.width = diff_x;
            }
            if (diff_y > 0) {
                m_selection_rect.height = diff_y;
            }
        }
    }

    for (auto entity : m_lifeforms) {
        entity->handle_event(event);
    }
}

void World::update(uint32_t elapsed)
{
    for (auto entity : m_lifeforms) {
        entity->update(elapsed);
    }
}

void World::refresh_texture()
{
    SDL_SetRenderTarget(m_renderer.get(), m_texture.get());
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    WorldRect viewport(m_viewport->get_rect());
    int padding = TILE_WIDTH * 2;

    m_txt_rect = geom::rect::move_inside(
                     geom::rect::move_by(
                         geom::rect::enlarge(viewport, padding),
                         WorldPoint(-1 * (viewport.x % TILE_WIDTH),
                                    -1 * (viewport.y % TILE_HEIGHT))),
                     WorldRect(0, 0,
                               WORLD_WIDTH * TILE_WIDTH,
                               WORLD_HEIGHT * TILE_HEIGHT));

    for (int i = 0; i <= m_txt_rect.width/TILE_WIDTH; i++) {
        int tile_x_pos = i + m_txt_rect.x/TILE_WIDTH;
        if (tile_x_pos >= WORLD_WIDTH || tile_x_pos < 0) {
            continue;
        }
        for (int j = 0; j <= m_txt_rect.height/TILE_HEIGHT; j++) {
            int tile_y_pos = j + m_txt_rect.y/TILE_HEIGHT;
            if (tile_y_pos >= WORLD_HEIGHT || tile_y_pos < 0) {
                continue;
            }
            SDL_Rect rect {i * TILE_WIDTH - (m_txt_rect.x % TILE_WIDTH),
                           j * TILE_HEIGHT - (m_txt_rect.y % TILE_HEIGHT),
                           TILE_WIDTH, TILE_HEIGHT};
            SDL_RenderCopy(m_renderer.get(), m_tiles.at(tile_x_pos, tile_y_pos)->terrain()->get_texture(),
                           nullptr, &rect);
        }
    }

    SDL_SetRenderTarget(m_renderer.get(), nullptr);

    SDL_Rect srect = m_txt_rect.as_sdl_rect();
    SDL_Rect vrect = viewport.as_sdl_rect();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "vrect{%d, %d, %d, %d} m_txt_rect{%d, %d, %d, %d}", vrect.x, vrect.y, vrect.w, vrect.h, srect.x, srect.y, srect.w, srect.h);
}

std::vector<WorldPoint> World::as_world_path(const std::vector<GridLocation> &path) const
{
    std::vector<WorldPoint> result;
    int current_x, current_y;
    int dir = 0; // 1 - right, 2 - down, 3 - left, 4 - up
    std::tie(current_x, current_y) = path.front();
    result.emplace_back(current_x * TILE_WIDTH + TILE_WIDTH/2,
                        current_y * TILE_HEIGHT + TILE_HEIGHT/2);
    for (auto iter=path.begin() + 1; iter!= path.end(); ++iter) {
        int pos_x, pos_y;
        std::tie(pos_x, pos_y) = *iter;
        if (pos_y == current_y && pos_x > current_x && dir != 1) {
            if (dir == 2) {
                result.emplace_back(pos_x * TILE_WIDTH - LifeForm::width/2,
                                    pos_y * TILE_HEIGHT + LifeForm::height/2);
            } else if (dir == 4) {
                result.emplace_back((current_x + 1)* TILE_WIDTH - LifeForm::width/2,
                                    (pos_y + 1) * TILE_HEIGHT - LifeForm::height/2);
            }
            dir = 1;
        } else if (pos_x == current_x && pos_y > current_y && dir != 2) {
            if (dir == 1) {
                result.emplace_back(pos_x * TILE_WIDTH + LifeForm::width/2,
                                    pos_y * TILE_HEIGHT - LifeForm::height/2);
            } else if (dir == 3) {
                result.emplace_back((pos_x + 1) * TILE_WIDTH - LifeForm::width/2,
                                    pos_y * TILE_HEIGHT - LifeForm::height/2);
            }
            dir = 2;
        } else if (pos_y == current_y && pos_x < current_x && dir != 3) {
            if (dir == 2) {
                result.emplace_back(current_x * TILE_WIDTH + LifeForm::width/2,
                                    pos_y * TILE_HEIGHT + LifeForm::height/2);
            } else if (dir == 4) {
                result.emplace_back(current_x * TILE_WIDTH + LifeForm::width/2,
                                    (pos_y + 1) * TILE_HEIGHT - LifeForm::height/2);
            }
            dir = 3;
        } else if (pos_x == current_x && pos_y < current_y && dir != 4) {
            if (dir == 1) {
                result.emplace_back(pos_x * TILE_WIDTH + LifeForm::width/2,
                                    current_y * TILE_HEIGHT + LifeForm::height/2);
            } else if (dir == 3) {
                result.emplace_back((pos_x + 1) * TILE_WIDTH - LifeForm::width/2,
                                    current_y * TILE_HEIGHT + LifeForm::height/2);
            }
            dir = 4;
        } else {
            printf("Oops.\n");
        }
        current_x = pos_x; current_y = pos_y;
    }
    std::tie(current_x, current_y) = path.back();
    result.emplace_back(current_x * TILE_WIDTH + TILE_WIDTH/2,
                        current_y * TILE_HEIGHT + TILE_HEIGHT/2);
    return result;
}

void World::render()
{
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    WorldRect viewport(m_viewport->get_rect());
    if (!viewport.is_inside(m_txt_rect)) {
        refresh_texture();
    }

    SDL_Rect rect  = geom::rect::relative_intersection(m_txt_rect, viewport).as_sdl_rect();
    SDL_RenderCopy(m_renderer.get(), m_texture.get(),
                   &rect, nullptr);

    for (auto entity : m_lifeforms) {
        entity->render(m_renderer.get());
    }

    if (m_selection_rect.width > 0 && m_selection_rect.height > 0) {
        WorldRect vrect = m_viewport->get_rect();
        SDL_Rect rect = geom::rect::move_by(m_selection_rect, WorldPoint(-1 * vrect.x, -1 * vrect.y)).as_sdl_rect();
        SDL_SetRenderDrawColor(m_renderer.get(), 255, 0, 0, 255);
        SDL_RenderDrawRect(m_renderer.get(), &rect);
    }

    SDL_RenderPresent(m_renderer.get());

    // Calculate FPS
    if (g_last_ticks) {
        uint32_t current_ticks(SDL_GetTicks());
        uint32_t delta = current_ticks - g_last_ticks;
        if (delta < 1000) {
            g_fps++;
        } else {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "fps: %d", g_fps);
            g_fps = 0;
            g_last_ticks = current_ticks;
        }
    } else {
        g_last_ticks = SDL_GetTicks();
    }
}
