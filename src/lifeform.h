#ifndef LIFEFORM_H
#define LIFEFORM_H

class SDL_Renderer;

class LifeForm {
public:
    LifeForm(double pos_x, double pos_y);

    void update(uint32_t elapsed);
    void render(SDL_Renderer* renderer);

private:
    double m_pos_x;
    double m_pos_y;
};

#endif // LIFEFORM_H
