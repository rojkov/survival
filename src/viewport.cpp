#include "viewport.h"
#include "worldpoint.h"
#include "gameconstants.h"

Viewport::Viewport(const WorldRect& rect)
    : m_rect(rect)
{
}

const WorldRect Viewport::get_rect() const
{
    return m_rect;
}

const ScreenRect Viewport::to_screen_rect(const WorldRect& rect) const
{
    WorldRect srect(geom::rect::move_by(rect, WorldPoint(-1 * m_rect.x, -1 * m_rect.y)));
    return ScreenRect(srect.x, srect.y, srect.width, srect.height);
}

void Viewport::move(const WorldPoint &delta)
{
    WorldRect new_rect(geom::rect::move_by(m_rect, delta));
    if (new_rect.is_inside(WorldRect(0, 0,
                                     WORLD_WIDTH * TILE_WIDTH,
                                     WORLD_HEIGHT * TILE_HEIGHT))) {
        m_rect = new_rect;
    }
}
