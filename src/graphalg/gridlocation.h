#ifndef GRIDLOCATION_H
#define GRIDLOCATION_H

#include <tuple>

using GridLocation = std::tuple<int, int>;

namespace std {
template <>
struct hash<GridLocation> {
    inline size_t operator()(const GridLocation& location) const {
        int x, y;
        tie (x, y) = location;
        return y * 1812433253 + x;
    }
};
}

#endif // GRIDLOCATION_H
