#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

struct SDL_Rect;

template<typename T>
struct BasePoint {
    T x;
    T y;

    BasePoint(const T& a_x, const T& a_y) : x(a_x), y(a_y) {};

    T abs() const { return std::sqrt(x * x + y * y); };
};

using Point = BasePoint<int>;

struct WorldPoint : BasePoint<int>
{
    WorldPoint(const int& a_x, const int& a_y) : BasePoint<int>(a_x, a_y) {};
};

struct Rect {
    int x, y, width, height;

    Rect(const int a_x, const int a_y, int a_width, int a_height) : x(a_x), y(a_y), width(a_width), height(a_height) {};

    bool is_inside(const Rect &rect) const;
    bool contains(const Point &point) const;
    Rect get_relative_intersection(const Rect &rect) const;
    SDL_Rect as_sdl_rect() const;

    const Rect move(const Point &delta) const;
    const Rect move_inside(const Rect &big_rect) const;
    const Rect enlarge(const int &padding) const;
};

namespace geom {

template<typename Vector_T>
Vector_T sum(const Vector_T& first, const Vector_T& second) {
    return Vector_T(first.x + second.x, first.y + second.y);
}

template<typename Vector_T>
Vector_T substruct(const Vector_T& first, const Vector_T& second) {
    return Vector_T(first.x - second.x, first.y - second.y);
}

template<typename Vector_T>
Vector_T scale(const Vector_T& vector, const double& s) {
    return Vector_T(vector.x * s, vector.y * s);
}

}

#endif // GEOMETRY_H
