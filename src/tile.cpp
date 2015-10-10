#include "assert.h"
#include "terrain.h"
#include "tile.h"

Tile::Tile(Terrain* terrain)
    : m_terrain(terrain)
    , m_region(0)
{
    assert(m_terrain != nullptr);
}

Terrain* Tile::terrain() const
{
    return m_terrain;
}

bool Tile::passable() const
{
    return m_terrain->passable();
}

uint32_t Tile::region() const
{
    return m_region;
}

void Tile::set_region(uint32_t reg)
{
    assert(reg > 0);
    m_region = reg;
}

