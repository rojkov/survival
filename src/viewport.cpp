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
    m_offset_x += delta_x;
    m_offset_y += delta_y;
}

SDL_Rect Viewport::get_rect() const
{
    return SDL_Rect {m_offset_x, m_offset_y, m_width, m_height};
}
