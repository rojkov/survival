#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>

struct SDL_Rect;

template<typename T>
class BasePoint {
public:
    BasePoint(T x, T y) : m_x(x), m_y(y) {};
    T x() const { return m_x; };
    T y() const { return m_y; };
    BasePoint sum(const BasePoint& delta) const { return BasePoint(m_x + delta.m_x, m_y + delta.m_y); };
    BasePoint substruct(const BasePoint& other) const { return BasePoint(m_x - other.m_x, m_y - other.m_y); };

    T abs() const { return sqrt(m_x*m_x + m_y*m_y); };
    BasePoint scale(double s) const { return BasePoint(s * m_x, s * m_y); };

private:
    T m_x;
    T m_y;
};

using Point = BasePoint<int>;
using WorldPoint = BasePoint<double>;

class Size {
public:
    Size(int width, int height) : m_width(width), m_height(height) {};
    int width() const { return m_width; };
    int height() const { return m_height; };
    Size sum(const Size &size) const { return Size(m_width + size.m_width, m_height + size.m_height); };

private:
    int m_width;
    int m_height;
};

class Rect {
public:
    Rect(int x, int y, int width, int height) : m_offset(x, y), m_size(width, height) {};
    Rect(Point offset, Size size) : m_offset(offset), m_size(size) {};

    Size size() const { return m_size; };
    Point offset() const { return m_offset; };
    bool is_inside(const Rect &rect) const;
    bool contains(const Point &point) const;
    Rect get_relative_intersection(const Rect &rect) const;
    SDL_Rect as_sdl_rect() const;

    const Rect move(const Point &delta) const;
    const Rect move_inside(const Rect &big_rect) const;

private:
    Point m_offset;
    Size m_size;
};

#endif // GEOMETRY_H
