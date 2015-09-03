#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include "lifeform.h"
#include "terrain.h"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50

class Viewport;

class World
{
public:
    World(std::shared_ptr<SDL_Renderer> renderer);

    void update(uint32_t elapsed);
    void render(const Viewport &viewport);

private:
    std::shared_ptr<SDL_Renderer> m_renderer;
    std::unique_ptr<LifeForm> m_lifeform;
    std::unique_ptr<Terrain> m_grass_terrain;
    std::unique_ptr<Terrain> m_water_terrain;
    std::array<Terrain*, WORLD_WIDTH * WORLD_HEIGHT> m_tiles;
};

#endif
