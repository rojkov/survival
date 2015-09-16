#include "geometry.h"
#include <SDL_rect.h>

bool Rect::contains(const Point &point) const
{
    if (point.x >= m_offset.x && point.x <= m_offset.x + m_size.width() &&
        point.y >= m_offset.y && point.y <= m_offset.y + m_size.height())
    {
        return true;
    } else {
        return false;
    }
}

bool Rect::is_inside(const Rect &rect) const
{
    int offset_x = m_offset.x, offset_y = m_offset.y,
        rect_offset_x = rect.m_offset.x, rect_offset_y = rect.m_offset.y;
    return offset_x >= rect_offset_x &&
           offset_y >= rect_offset_y &&
           offset_x + m_size.width() <= rect_offset_x + rect.m_size.width() &&
           offset_y + m_size.height() <= rect_offset_y + rect.m_size.height();
}

Rect Rect::get_relative_intersection(const Rect &rect) const
{
    // make the intersection relative to this Rect
    int x = rect.m_offset.x - m_offset.x;
    int y = rect.m_offset.y - m_offset.y;

    int width = rect.m_size.width();
    int height = rect.m_size.height();
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (width > m_size.width()) {
        width = m_size.width();
    }
    if (height > m_size.height()) {
        height = m_size.height();
    }
    return Rect(x, y, width, height);
}

SDL_Rect Rect::as_sdl_rect() const
{
    SDL_Rect sdlrect {
        m_offset.x,
        m_offset.y,
        m_size.width(),
        m_size.height()
    };

    return sdlrect;
}

const Rect Rect::move(const Point &delta) const
{
    return Rect(m_offset.sum(delta), m_size);
}

const Rect Rect::move_inside(const Rect &big_rect) const
{
    int x(m_offset.x), y(m_offset.y);

    if (m_offset.x < big_rect.m_offset.x) {
        x = big_rect.m_offset.x;
    } else if (m_offset.x + m_size.width() >
               big_rect.m_offset.x + big_rect.m_size.width()) {
        x = big_rect.m_size.width() - m_size.width();
    }

    if (m_offset.y < big_rect.m_offset.y) {
        y = big_rect.m_offset.y;
    } else if (m_offset.y + m_size.height() >
               big_rect.m_offset.y + big_rect.m_size.height()) {
        y = big_rect.m_size.height() - m_size.height();
    }
    return Rect(Point(x, y), m_size);
}
