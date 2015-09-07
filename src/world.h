#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <array>
#include <vector>
#include <tuple>
#include "lifeform.h"
#include "terrain.h"
#include "geometry.h"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50

class Viewport;

class World
{
public:
    World(std::shared_ptr<SDL_Renderer> renderer, const Rect &viewport_rect);

    void update(uint32_t elapsed);
    void render(const Rect &viewport);

    typedef std::tuple<int, int> Location;
    std::vector<Location> neighbors(Location loc);
    int cost(Location a, Location b) { return 1; };

private:

    inline bool in_bounds(Location loc) {
        int x, y;
        std::tie(x, y) = loc;
        return x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT;
    };

    inline bool passable(Location loc) {
        int x, y;
        std::tie(x, y) = loc;
        return m_tiles.at(y * WORLD_HEIGHT + x)->passable();
    };

    void refresh_texture(const Rect &viewport);

    std::shared_ptr<SDL_Renderer> m_renderer;
    std::unique_ptr<LifeForm> m_lifeform;
    std::unique_ptr<Terrain> m_grass_terrain;
    std::unique_ptr<Terrain> m_water_terrain;
    std::array<Terrain*, WORLD_WIDTH * WORLD_HEIGHT> m_tiles;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
    Rect m_txt_rect;

    static std::array<Location, 4> DIRS;
};

#endif
