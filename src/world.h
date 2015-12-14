#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include "worldpoint.h"
#include "worldrect.h"
#include "graphalg/gridgraph.h"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 50

class Viewport;
class Terrain;
class Tile;
class LifeForm;
struct WorldPosition;

using WorldGrid = GridGraph<Tile, WORLD_WIDTH, WORLD_HEIGHT>;

class World
{
public:
    World(std::shared_ptr<SDL_Renderer> renderer);
    virtual ~World();

    std::vector<WorldPoint> get_path(const WorldPosition& start, const WorldPosition& end) const;
    GridLocation location(const WorldPosition& pos) const;
    GridLocation closest(const GridLocation& loc, const std::unordered_set<GridLocation>& locs) const;
    const WorldRect get_viewport() const;
    SDL_Rect to_sdl_rect(const WorldRect& rect) const;

    void add_entity(std::shared_ptr<LifeForm> entity);

    void handle_event(const SDL_Event &event);
    void update(uint32_t elapsed);
    void render();

private:

    void refresh_texture();
    std::vector<WorldPoint> as_world_path(const std::vector<GridLocation> &path) const;

    std::shared_ptr<SDL_Renderer> m_renderer;
    std::shared_ptr<Viewport> m_viewport;
    std::vector<std::shared_ptr<LifeForm> > m_lifeforms;
    std::unique_ptr<Terrain> m_grass_terrain;
    std::unique_ptr<Terrain> m_water_terrain;
    WorldGrid m_tiles;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
    WorldRect m_txt_rect;
    WorldRect m_selection_rect; // Selected region in world coordinates
    bool m_mouse_down; // TODO: proper FSM is needed
};

#endif
