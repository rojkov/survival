#ifndef GRIDLOCATION_H
#define GRIDLOCATION_H

#include <tuple>

typedef std::tuple<int, int> GridLocation;

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
