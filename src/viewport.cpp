#include "viewport.h"

Viewport::Viewport(Rect rect)
    : m_rect(rect)
{
}

const Rect Viewport::get_rect() const
{
    return m_rect;
}

void Viewport::move(const Point &delta)
{
    Rect new_rect(m_rect.move(delta));
    if (new_rect.is_inside(Rect(0, 0, 50 * 16, 50 * 16))) {
        m_rect = new_rect;
    }
}
