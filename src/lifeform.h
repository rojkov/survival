#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <memory>
#include <queue>
#include <unordered_set>
#include "commands/command.h"
#include "graphalg/gridlocation.h"

class World;
struct WorldPosition;
class SDL_Renderer;
union SDL_Event;

class LifeForm {
public:
    LifeForm(std::weak_ptr<World> world, double pos_x, double pos_y);

    WorldPosition get_pos() const;
    void move_to(const WorldPosition &new_position);

    bool focused() const { return m_focused; };
    void set_focused (bool focused) { m_focused = focused; };
    void patrol(const std::unordered_set<GridLocation>& locations);

    void handle_event(const SDL_Event &event);
    void update(uint32_t elapsed);
    void render(SDL_Renderer* renderer);

private:
    std::weak_ptr<World> m_world;
    double m_pos_x;
    double m_pos_y;
    bool m_focused;
    std::queue<std::unique_ptr<Command> > m_commands;
    std::unordered_set<GridLocation> m_visited_tiles;
    std::unordered_set<GridLocation> m_unvisited_tiles;
};

#endif // LIFEFORM_H
