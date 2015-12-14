#ifndef WORLDRECT_H
#define WORLDRECT_H

#include "geometry.h"

struct WorldPoint;

struct WorldRect : BaseRect<int32_t, WorldPoint>
{
    WorldRect(const int32_t& a_x, const int32_t& a_y,
              const int32_t& a_width, const int32_t& a_height)
        : BaseRect<int32_t, WorldPoint>(a_x, a_y, a_width, a_height) {};
};

#endif // WORLDRECT_H
