#include "terrain.h"
#include "assert.h"

Terrain::Terrain(TerrainType type, SDL_Texture* texture)
    : m_type(type)
    , m_texture(texture, SDL_DestroyTexture)
{
    assert(m_texture != nullptr);
}

bool Terrain::passable() const
{
    return m_type == GRASS;
}

SDL_Texture * const Terrain::get_texture() const
{
    return m_texture.get();
}
