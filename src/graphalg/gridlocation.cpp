#include <cmath>

#include "gridlocation.h"

double grid_distance(const GridLocation& loc1, const GridLocation& loc2)
{
    int x1, y1, x2, y2;
    std::tie(x1, y1) = loc1;
    std::tie(x2, y2) = loc2;
    int xdiff(x2 - x1);
    int ydiff(y2 - y1);
    return std::sqrt((xdiff * xdiff) + (ydiff * ydiff));
}
