#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include "geometry.h"
#include "graphalg/gridgraph.h"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50

class Viewport;
class Terrain;
class LifeForm;

using WorldGrid = GridGraph<Terrain, WORLD_WIDTH, WORLD_HEIGHT>;

class World
{
public:
    World(std::shared_ptr<SDL_Renderer> renderer);
    virtual ~World();

    std::vector<Point> get_path(const WorldPoint &start, const WorldPoint &end) const;
    const Rect get_viewport() const;

    void add_entity(std::shared_ptr<LifeForm> entity);

    void handle_event(const SDL_Event &event);
    void update(uint32_t elapsed);
    void render();

private:

    void refresh_texture();
    std::vector<Point> as_world_path(const std::vector<GridLocation> &path) const;

    std::shared_ptr<SDL_Renderer> m_renderer;
    std::shared_ptr<Viewport> m_viewport;
    std::vector<std::shared_ptr<LifeForm> > m_lifeforms;
    std::unique_ptr<Terrain> m_grass_terrain;
    std::unique_ptr<Terrain> m_water_terrain;
    WorldGrid m_tiles;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
    Rect m_txt_rect;
};

#endif
