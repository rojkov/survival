#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <memory>
#include <queue>
#include "geometry.h"
#include "commands/command.h"

class World;
class SDL_Renderer;
union SDL_Event;

class LifeForm {
public:
    LifeForm(std::weak_ptr<World> world, double pos_x, double pos_y);

    WorldPoint get_pos() const;
    void move_to(const WorldPoint &new_position);

    bool focused() const { return m_focused; };
    void set_focused (bool focused) { m_focused = focused; };

    void handle_event(const SDL_Event &event);
    void update(uint32_t elapsed);
    void render(SDL_Renderer* renderer);

private:
    std::weak_ptr<World> m_world;
    double m_pos_x;
    double m_pos_y;
    bool m_focused;
    std::queue<std::shared_ptr<Command> > m_commands;
};

#endif // LIFEFORM_H
