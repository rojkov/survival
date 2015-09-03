#ifndef VIEWPORT_H
#define VIEWPORT_H

struct SDL_Rect;

class Viewport
{
public:
    Viewport(int offset_x, int offset_y, int width, int height);

    void move(int delta_x, int delta_y);
    SDL_Rect get_rect() const;

private:
    int m_offset_x;
    int m_offset_y;
    int m_width;
    int m_height;
};

#endif // VIEWPORT_H
