#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

struct SDL_Rect;

template<typename T>
struct BasePoint {
    T x;
    T y;

    BasePoint(const T& a_x, const T& a_y) : x(a_x), y(a_y) {};
    BasePoint sum(const BasePoint& delta) const { return BasePoint(x + delta.x, y + delta.y); };
    BasePoint substruct(const BasePoint& other) const { return BasePoint(x - other.x, y - other.y); };

    T abs() const { return std::sqrt(x * x + y * y); };
    BasePoint scale(double s) const { return BasePoint(s * x, s * y); };
};

using Point = BasePoint<int>;
using WorldPoint = BasePoint<double>;

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

#endif // GEOMETRY_H
