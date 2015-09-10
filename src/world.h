#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <queue>
#include "lifeform.h"
#include "terrain.h"
#include "geometry.h"
#include "graphalg/gridgraph.h"
#include "commands/command.h"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50

class Viewport;

typedef GridGraph<Terrain, WORLD_WIDTH, WORLD_HEIGHT> WorldGrid;

class World
{
public:
    World(std::shared_ptr<SDL_Renderer> renderer, const Rect &viewport_rect);

    void update(uint32_t elapsed);
    void render(const Rect &viewport);

private:

    void refresh_texture(const Rect &viewport);
    std::vector<Point> as_world_path(const std::vector<GridLocation> &path);

    std::shared_ptr<SDL_Renderer> m_renderer;
    std::shared_ptr<LifeForm> m_lifeform;
    std::unique_ptr<Terrain> m_grass_terrain;
    std::unique_ptr<Terrain> m_water_terrain;
    WorldGrid m_tiles;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
    Rect m_txt_rect;
    std::queue<std::shared_ptr<Command> > m_commands;
};

#endif
