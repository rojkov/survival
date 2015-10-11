#ifndef TILE_H
#define TILE_H

class Terrain;

class Tile
{
public:
    Tile(Terrain* terrain);

    Terrain* terrain() const;

    bool passable() const;

    uint32_t region() const;
    void set_region(uint32_t reg);

    bool is_same_type(const Tile& other) const;

private:
    Terrain* const m_terrain;
    uint32_t m_region;
};

#endif // TILE_H
