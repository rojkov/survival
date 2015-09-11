#ifndef LIFEFORM_H
#define LIFEFORM_H

#include "geometry.h"

class SDL_Renderer;
union SDL_Event;

class LifeForm {
public:
    LifeForm(double pos_x, double pos_y);

    WorldPoint get_pos() const;
    void move_to(const WorldPoint &new_position);

    bool focused() const { return m_focused; };
    void set_focused (bool focused) { m_focused = focused; };

    void update(uint32_t elapsed);
    void render(SDL_Renderer* renderer);

private:
    double m_pos_x;
    double m_pos_y;
    bool m_focused;
};

#endif // LIFEFORM_H
