#ifndef TERRAIN_H
#define TERRAIN_H

#include <memory>
#include <SDL.h>

class Terrain
{
public:
    enum TerrainType {
        GRASS = 1,
        WATER
    };

    Terrain(TerrainType type, SDL_Texture* texture);

    SDL_Texture * const get_texture() const;

private:
    TerrainType m_type;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
};

#endif // TERRAIN_H
