#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <string>
#include <vector>
#include "world.h"
#include "terrain.h"
#include "lifeform.h"
#include "viewport.h"
#include "graphalg/a_star_search.h"

static uint32_t g_last_ticks = 0;
static int g_fps = 0;

typedef std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> unique_surf;

inline int heuristic(GridLocation a, GridLocation b) {
    int x1, y1, x2, y2;
    std::tie(x1, y1) = a;
    std::tie(x2, y2) = b;
    return abs(x1 - x2) + abs(y1 + y2);
}

World::World(std::shared_ptr<SDL_Renderer> renderer)
    : m_renderer(renderer)
    , m_viewport(std::make_shared<Viewport>(Rect(0, 0, 640, 480)))
    , m_grass_terrain(nullptr)
    , m_water_terrain(nullptr)
    , m_texture(nullptr, SDL_DestroyTexture)
    , m_txt_rect(Rect(0, 0, 640 + 16*4, 480 + 16*4))
{
    unique_surf temp_surf(IMG_Load("tileset.png"), SDL_FreeSurface);
    assert(temp_surf != nullptr);
    unique_surf grass_surf(SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0),
                           SDL_FreeSurface);
    assert(grass_surf != nullptr);
    unique_surf water_surf(SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0),
                           SDL_FreeSurface);
    assert(water_surf != nullptr);

    SDL_Rect rect {0, 0, 16, 16};
    SDL_BlitSurface(temp_surf.get(), &rect, grass_surf.get(), nullptr);
    rect.x = 16;
    SDL_BlitSurface(temp_surf.get(), &rect, water_surf.get(), nullptr);

    m_grass_terrain.reset(new Terrain(Terrain::GRASS, SDL_CreateTextureFromSurface(m_renderer.get(), grass_surf.get())));
    m_water_terrain.reset(new Terrain(Terrain::WATER, SDL_CreateTextureFromSurface(m_renderer.get(), water_surf.get())));

    m_tiles.load("world.map", [this](std::string token) -> Terrain* {
            switch (std::stoi(token)) {
            case Terrain::GRASS:
                return m_grass_terrain.get();
            case Terrain::WATER:
                return m_water_terrain.get();
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

std::vector<Point> World::get_path(const WorldPoint &start, const WorldPoint &end) const
{
    GridLocation current {(int)round(start.x)/16, (int)round(start.y)/16};
    GridLocation goal {(int)round(end.x)/16, (int)round(end.y)/16};
    std::function<int(GridLocation, GridLocation)> h_func = heuristic;
    auto path = a_star_search(m_tiles, current, goal, h_func);
    auto wpath = as_world_path(path);
    return wpath;
}

void World::add_entity(std::shared_ptr<LifeForm> entity)
{
    m_lifeforms.push_back(entity);
}

void World::handle_event(const SDL_Event &event)
{
    const uint8_t* current_key_states = SDL_GetKeyboardState(nullptr);
    if (current_key_states[SDL_SCANCODE_UP]) {
        m_viewport->move(Point(0, -1));
    } else if (current_key_states[SDL_SCANCODE_DOWN]) {
        m_viewport->move(Point(0, 1));
    } else if (current_key_states[SDL_SCANCODE_LEFT]) {
        m_viewport->move(Point(-1, 0));
    } else if (current_key_states[SDL_SCANCODE_RIGHT]) {
        m_viewport->move(Point(1, 0));
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

    Rect viewport(m_viewport->get_rect());
    int padding = 16 * 2;
    m_txt_rect = viewport.enlarge(padding)
                         .move(Point(-1 * (viewport.x % 16),
                                     -1 * (viewport.y % 16)))
                         .move_inside(Rect(0, 0, WORLD_WIDTH * 16, WORLD_HEIGHT * 16));

    for (int i = 0; i <= m_txt_rect.width/16; i++) {
        int tile_x_pos = i + m_txt_rect.x/16;
        if (tile_x_pos >= WORLD_WIDTH || tile_x_pos < 0) {
            continue;
        }
        for (int j = 0; j <= m_txt_rect.height/16; j++) {
            int tile_y_pos = j + m_txt_rect.y/16;
            if (tile_y_pos >= WORLD_HEIGHT || tile_y_pos < 0) {
                continue;
            }
            SDL_Rect rect {i * 16 - (m_txt_rect.x % 16),
                           j * 16 - (m_txt_rect.y % 16), 16, 16};
            SDL_RenderCopy(m_renderer.get(), m_tiles.at(tile_x_pos, tile_y_pos)->get_texture(),
                           nullptr, &rect);
        }
    }

    SDL_SetRenderTarget(m_renderer.get(), nullptr);

    SDL_Rect srect = m_txt_rect.as_sdl_rect();
    SDL_Rect vrect = viewport.as_sdl_rect();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "vrect{%d, %d, %d, %d} m_txt_rect{%d, %d, %d, %d}", vrect.x, vrect.y, vrect.w, vrect.h, srect.x, srect.y, srect.w, srect.h);
}

std::vector<Point> World::as_world_path(const std::vector<GridLocation> &path) const
{
    std::vector<Point> result;
    int current_x, current_y;
    int dir = 0; // 1 - right, 2 - down, 3 - left, 4 - up
    std::tie(current_x, current_y) = path.front();
    result.emplace_back(current_x * 16 + 16/2, current_y * 16 + 16/2);
    for (auto iter=path.begin() + 1; iter!= path.end(); ++iter) {
        int pos_x, pos_y;
        std::tie(pos_x, pos_y) = *iter;
        if (pos_y == current_y && pos_x > current_x && dir != 1) {
            if (dir == 2) {
                // 8 - width of lifeform
                result.emplace_back(pos_x * 16 - 8/2,
                                    pos_y * 16 + 8/2);
            } else if (dir == 4) {
                result.emplace_back(current_x * 16 + 8/2,
                                    pos_y * 16 + 8/2);
            }
            dir = 1;
        } else if (pos_x == current_x && pos_y > current_y && dir != 2) {
            if (dir == 1) {
                result.emplace_back(pos_x * 16 + 8/2,
                                    pos_y * 16 - 8/2);
            } else if (dir == 3) {
                result.emplace_back(pos_x * 16 - 8/2,
                                    pos_y * 16 - 8/2);
            }
            dir = 2;
        } else if (pos_y == current_y && pos_x < current_x && dir != 3) {
            if (dir == 2) {
                result.emplace_back(current_x * 16 - 8/2,
                                    pos_y * 16 + 8/2);
            } else if (dir == 4) {
                result.emplace_back(current_x * 16 + 8/2,
                                    pos_y * 16 - 8/2);
            }
            dir = 3;
        } else if (pos_x == current_x && pos_y < current_y && dir != 4) {
            if (dir == 1) {
                result.emplace_back(pos_x * 16 + 8/2,
                                    current_y * 16 + 8/2);
            } else if (dir == 3) {
                result.emplace_back(pos_x * 16 - 8/2,
                                    current_y * 16 - 8/2);
            }
            dir = 4;
        } else {
            printf("Oops.\n");
        }
        current_x = pos_x; current_y = pos_y;
    }
    std::tie(current_x, current_y) = path.back();
    result.emplace_back(current_x * 16 + 16/2, current_y * 16 + 16/2);
    return result;
}

void World::render()
{
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    Rect viewport(m_viewport->get_rect());
    if (!viewport.is_inside(m_txt_rect)) {
        refresh_texture();
    }

    SDL_Rect rect  = m_txt_rect.get_relative_intersection(viewport).as_sdl_rect();
    SDL_RenderCopy(m_renderer.get(), m_texture.get(),
                   &rect, nullptr);

    for (auto entity : m_lifeforms) {
        entity->render(m_renderer.get());
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
