#include "geometry.h"
#include <SDL_rect.h>

bool Rect::contains(const Point &point) const
{
    if (point.x >= offset.x && point.x <= offset.x + size.width &&
        point.y >= offset.y && point.y <= offset.y + size.height)
    {
        return true;
    } else {
        return false;
    }
}

bool Rect::is_inside(const Rect &rect) const
{
    int offset_x = offset.x, offset_y = offset.y,
        rect_offset_x = rect.offset.x, rect_offset_y = rect.offset.y;
    return offset_x >= rect_offset_x &&
           offset_y >= rect_offset_y &&
           offset_x + size.width <= rect_offset_x + rect.size.width &&
           offset_y + size.height <= rect_offset_y + rect.size.height;
}

Rect Rect::get_relative_intersection(const Rect &rect) const
{
    // make the intersection relative to this Rect
    int x = rect.offset.x - offset.x;
    int y = rect.offset.y - offset.y;

    int width = rect.size.width;
    int height = rect.size.height;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (width > size.width) {
        width = size.width;
    }
    if (height > size.height) {
        height = size.height;
    }
    return Rect(x, y, width, height);
}

SDL_Rect Rect::as_sdl_rect() const
{
    SDL_Rect sdlrect {
        offset.x,
        offset.y,
        size.width,
        size.height
    };

    return sdlrect;
}

const Rect Rect::move(const Point &delta) const
{
    return Rect(offset.sum(delta), size);
}

const Rect Rect::move_inside(const Rect &big_rect) const
{
    int x(offset.x), y(offset.y);

    if (offset.x < big_rect.offset.x) {
        x = big_rect.offset.x;
    } else if (offset.x + size.width >
               big_rect.offset.x + big_rect.size.width) {
        x = big_rect.size.width - size.width;
    }

    if (offset.y < big_rect.offset.y) {
        y = big_rect.offset.y;
    } else if (offset.y + size.height >
               big_rect.offset.y + big_rect.size.height) {
        y = big_rect.size.height - size.height;
    }
    return Rect(Point(x, y), size);
}
