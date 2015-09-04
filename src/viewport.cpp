#include "viewport.h"
#include <SDL.h>

Viewport::Viewport(int offset_x, int offset_y, int width, int height)
    : m_offset_x(offset_x)
    , m_offset_y(offset_y)
    , m_width(width)
    , m_height(height)
{
}

void Viewport::move(int delta_x, int delta_y)
{
    int new_offset_x = m_offset_x + delta_x;
    if (new_offset_x >= 0 && new_offset_x < 50 * 16 - 640) {
        m_offset_x = new_offset_x;
    }
    int new_offset_y = m_offset_y + delta_y;
    if (new_offset_y >= 0 && new_offset_y < 50 * 16 - 480) {
        m_offset_y = new_offset_y;
    }
}

SDL_Rect Viewport::get_rect() const
{
    return SDL_Rect {m_offset_x, m_offset_y, m_width, m_height};
}
