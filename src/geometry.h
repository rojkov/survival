#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

#include <assert.h>
#include <SDL_rect.h>

template<typename T>
struct BasePoint {
    T x;
    T y;

    BasePoint(const T& a_x, const T& a_y) : x(a_x), y(a_y) {};

    T abs() const { return std::sqrt(x * x + y * y); };
};

using Point = BasePoint<int>;

template<typename T, typename Vector_T>
struct BaseRect {
    T x, y, width, height;

    BaseRect(const T& a_x, const T& a_y, const T& a_width, const T& a_height)
      : x(a_x)
      , y(a_y)
      , width(a_width)
      , height(a_height) {};

    bool contains(const Vector_T& point) const
    {
        return point.x >= x &&
               point.x <= x + width &&
               point.y >= y &&
               point.y <= y + height;
    };

    bool is_inside(const BaseRect& other) const
    {
        return x >= other.x &&
               y >= other.y &&
               x + width <= other.x + other.width &&
               y + height <= other.y + other.height;
    };

    SDL_Rect as_sdl_rect() const
    {
        SDL_Rect sdlrect {
            static_cast<int>(x),
            static_cast<int>(y),
            static_cast<int>(width),
            static_cast<int>(height)
        };

        return sdlrect;
    };
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
Vector_T sum(const Vector_T& first, const Vector_T& second)
{
    return Vector_T(first.x + second.x, first.y + second.y);
}

template<typename Vector_T>
Vector_T substruct(const Vector_T& first, const Vector_T& second)
{
    return Vector_T(first.x - second.x, first.y - second.y);
}

template<typename Vector_T>
Vector_T scale(const Vector_T& vector, const double& s)
{
    return Vector_T(vector.x * s, vector.y * s);
}

namespace rect {

template<typename Rect_T>
const Rect_T relative_intersection(const Rect_T& first, const Rect_T& second)
{
    // make the intersection relative to this Rect
    auto new_x = second.x - first.x;
    auto new_y = second.y - first.y;

    auto new_width = second.width;
    auto new_height = second.height;

    if (new_x < 0) {
        new_width += new_x;
        new_x = 0;
    }
    if (new_y < 0) {
        new_height += new_y;
        new_y = 0;
    }
    if (new_width > first.width) {
        new_width = first.width;
    }
    if (new_height > first.height) {
        new_height = first.height;
    }
    return Rect_T(new_x, new_y, new_width, new_height);
}

template<typename Rect_T, typename Vector_T>
const Rect_T move_by(const Rect_T& rect, const Vector_T& delta)
{
    return Rect_T(rect.x + delta.x, rect.y + delta.y, rect.width, rect.height);
}

template<typename Rect_T>
const Rect_T move_inside(const Rect_T& rect, const Rect_T& big_rect)
{
    assert(big_rect.width >= rect.width);
    assert(big_rect.height >= rect.height);

    auto new_x(rect.x), new_y(rect.y);

    if (rect.x < big_rect.x) {
        new_x = big_rect.x;
    } else if (rect.x + rect.width > big_rect.x + big_rect.width) {
        new_x = big_rect.width - rect.width;
    }

    if (rect.y < big_rect.y) {
        new_y = big_rect.y;
    } else if (rect.y + rect.height > big_rect.y + big_rect.height) {
        new_y = big_rect.height - rect.height;
    }
    return Rect_T(new_x, new_y, rect.width, rect.height);
}

template<typename Rect_T>
const Rect_T enlarge(const Rect_T& rect, const int32_t& padding)
{
    return Rect_T(rect.x - padding,
                  rect.y - padding,
                  rect.width + 2*padding,
                  rect.height + 2*padding);
}

} // namespace rect

} // namespace geom

#endif // GEOMETRY_H
