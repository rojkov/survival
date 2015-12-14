#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "worldrect.h"

struct WorldPoint;

struct ScreenPoint : BasePoint<int32_t>
{
    ScreenPoint(const int32_t& a_x, const int32_t& a_y) : BasePoint<int32_t>(a_x, a_y) {};
};

struct ScreenRect : BaseRect<int32_t, ScreenPoint>
{
    ScreenRect(const int32_t& a_x, const int32_t& a_y,
              const int32_t& a_width, const int32_t& a_height)
        : BaseRect<int32_t, ScreenPoint>(a_x, a_y, a_width, a_height) {};
};

class Viewport
{
public:
    Viewport(const WorldRect& rect);

    const WorldRect get_rect() const;
    const ScreenRect to_screen_rect(const WorldRect& rect) const;

    void move(const WorldPoint &delta);

private:
    WorldRect m_rect;
};

#endif // VIEWPORT_H
