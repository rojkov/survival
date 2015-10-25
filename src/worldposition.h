#ifndef WORLDPOSITION_H
#define WORLDPOSITION_H

#include "geometry.h"

struct WorldPosition : BasePoint<double> {
    WorldPosition(const double& a_x, const double& a_y);
};

#endif // WORLDPOSITION_H
