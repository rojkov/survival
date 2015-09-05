#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <fstream>      // for std::ifstream
#include <sstream>      // for std::istringstream
#include <string>
#include "world.h"
#include "viewport.h"

static uint32_t g_last_ticks = 0;
static int g_fps = 0;

typedef std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> unique_surf;

World::World(std::shared_ptr<SDL_Renderer> renderer, const Rect &viewport_rect)
    : m_renderer(renderer)
    , m_lifeform(new LifeForm(50, 50))
    , m_grass_terrain(nullptr)
    , m_water_terrain(nullptr)
    , m_texture(nullptr, SDL_DestroyTexture)
    , m_txt_rect(Rect(viewport_rect.offset(), viewport_rect.size().sum(Size(16*4, 16*4))))
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

    std::ifstream mapFile("world.map");
    std::string line;

    size_t row(0), tile_idx(0);
    while (std::getline(mapFile, line)) {
        size_t column = 0;
        assert(row < WORLD_HEIGHT);
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            assert(column < WORLD_WIDTH);
            switch (std::stoi(token)) {
                case Terrain::GRASS:
                    m_tiles.at(tile_idx) = m_grass_terrain.get();
                    break;
                case Terrain::WATER:
                    m_tiles.at(tile_idx) = m_water_terrain.get();
                    break;
                default:
                    assert(false);
            }
            column++;
            tile_idx++;
        }
        row++;
    }

    // Create world texture
    Size size = m_txt_rect.size();
    m_texture.reset(SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      size.width(), size.height()));
    assert(m_texture != nullptr);
    refresh_texture(viewport_rect);
}

void World::update(uint32_t elapsed)
{
    m_lifeform->update(elapsed);
}

void World::refresh_texture(const Rect &viewport)
{
    SDL_SetRenderTarget(m_renderer.get(), m_texture.get());
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    int padding = 16 * 2;
    Rect txt_rect = Rect(Point(viewport.offset().sum(Point(-padding, -padding))),
                         Size(viewport.size().sum(Size(2*padding, 2*padding))));
    txt_rect.move(Point(-1 * (viewport.offset().x() % 16),
                        -1 * (viewport.offset().y() % 16)));

    txt_rect.move_inside(Rect(0, 0, WORLD_WIDTH * 16, WORLD_HEIGHT * 16));
    m_txt_rect = txt_rect;

    int offset_x = m_txt_rect.offset().x(), offset_y = m_txt_rect.offset().y();

    for (int i = 0; i <= m_txt_rect.size().width()/16; i++) {
        int tile_x_pos = i + offset_x/16;
        if (tile_x_pos >= WORLD_WIDTH || tile_x_pos < 0) {
            continue;
        }
        for (int j = 0; j <= m_txt_rect.size().height()/16; j++) {
            int tile_y_pos = j + offset_y/16;
            if (tile_y_pos >= WORLD_HEIGHT || tile_y_pos < 0) {
                continue;
            }
            SDL_Rect rect {i * 16 - (offset_x % 16), j * 16 - (offset_y % 16), 16, 16};
            SDL_RenderCopy(m_renderer.get(), m_tiles.at(tile_y_pos * WORLD_HEIGHT + tile_x_pos)->get_texture(),
                           nullptr, &rect);
        }
    }

    SDL_SetRenderTarget(m_renderer.get(), nullptr);

    SDL_Rect srect = m_txt_rect.as_sdl_rect();
    SDL_Rect vrect = viewport.as_sdl_rect();
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "vrect{%d, %d, %d, %d} m_txt_rect{%d, %d, %d, %d}", vrect.x, vrect.y, vrect.w, vrect.h, srect.x, srect.y, srect.w, srect.h);
}

void World::render(const Rect &viewport)
{
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    if (!viewport.is_inside(m_txt_rect)) {
        refresh_texture(viewport);
    }

    SDL_Rect rect  = m_txt_rect.get_relative_intersection(viewport).as_sdl_rect();
    SDL_RenderCopy(m_renderer.get(), m_texture.get(),
                   &rect, nullptr);

    m_lifeform->render(m_renderer.get());
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
