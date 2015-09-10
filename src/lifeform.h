#ifndef LIFEFORM_H
#define LIFEFORM_H

#include "geometry.h"

class SDL_Renderer;

class LifeForm {
public:
    LifeForm(double pos_x, double pos_y);

    WorldPoint get_pos() const;
    void move_to(const WorldPoint &new_position);

    void update(uint32_t elapsed);
    void render(SDL_Renderer* renderer);

private:
    double m_pos_x;
    double m_pos_y;
};

#endif // LIFEFORM_H
