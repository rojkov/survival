#ifndef WORLDPOINT_H
#define WORLDPOINT_H

#include "geometry.h"

struct WorldPoint : BasePoint<int32_t>
{
    WorldPoint(const int32_t& a_x, const int32_t& a_y) : BasePoint<int32_t>(a_x, a_y) {};
};

#endif // WORLDPOINT_H
