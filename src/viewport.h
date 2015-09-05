#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "geometry.h"

class Viewport
{
public:
    Viewport(Rect rect);

    const Rect get_rect() const;

    void move(const Point &delta);

private:
    Rect m_rect;
};

#endif // VIEWPORT_H
