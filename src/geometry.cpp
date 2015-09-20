#include "geometry.h"
#include <SDL_rect.h>

bool Rect::contains(const Point &point) const
{
    if (point.x >= x && point.x <= x + width &&
        point.y >= y && point.y <= y + height)
    {
        return true;
    } else {
        return false;
    }
}

bool Rect::is_inside(const Rect &rect) const
{
    return x >= rect.x &&
           y >= rect.y &&
           x + width <= rect.x + rect.width &&
           y + height <= rect.y + rect.height;
}

Rect Rect::get_relative_intersection(const Rect &rect) const
{
    // make the intersection relative to this Rect
    int new_x = rect.x - x;
    int new_y = rect.y - y;

    int new_width = rect.width;
    int new_height = rect.height;
    if (new_x < 0) {
        new_width += new_x;
        new_x = 0;
    }
    if (new_y < 0) {
        new_height += new_y;
        new_y = 0;
    }
    if (new_width > width) {
        new_width = width;
    }
    if (new_height > height) {
        new_height = height;
    }
    return Rect(new_x, new_y, new_width, new_height);
}

SDL_Rect Rect::as_sdl_rect() const
{
    SDL_Rect sdlrect {x, y, width, height};

    return sdlrect;
}

const Rect Rect::move(const Point &delta) const
{
    return Rect(x + delta.x, y + delta.y, width, height);
}

const Rect Rect::move_inside(const Rect &big_rect) const
{
    int new_x(x), new_y(y);

    if (x < big_rect.x) {
        new_x = big_rect.x;
    } else if (x + width > big_rect.x + big_rect.width) {
        new_x = big_rect.width - width;
    }

    if (y < big_rect.y) {
        new_y = big_rect.y;
    } else if (y + height > big_rect.y + big_rect.height) {
        new_y = big_rect.height - height;
    }
    return Rect(new_x, new_y, width, height);
}

const Rect Rect::enlarge(const int &padding) const
{
    return Rect(x - padding, y - padding, width + 2*padding, height + 2*padding);
}
