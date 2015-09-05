#ifndef GEOMETRY_H
#define GEOMETRY_H

struct SDL_Rect;

class Point {
public:
    Point(int x, int y) : m_x(x), m_y(y) {};
    int x() const { return m_x; };
    int y() const { return m_y; };
    Point sum(const Point& delta) const { return Point(m_x + delta.m_x, m_y + delta.m_y); };

    void set_x(int x) { m_x = x; };
    void set_y(int y) { m_y = y; };

private:
    int m_x;
    int m_y;
};

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
    Rect get_relative_intersection(const Rect &rect) const;
    SDL_Rect as_sdl_rect() const;

    void move(const Point &delta);
    void move_inside(const Rect &big_rect);

private:
    Point m_offset;
    Size m_size;
};

#endif // GEOMETRY_H
