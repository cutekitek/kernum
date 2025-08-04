#ifndef KERNUM_CHUNK_H
#define KERNUM_CHUNK_H
#define CHUNK_SIZE 64


#include "../../utils/FCoords.h"
#include "world/entities/EntityList.h"
#include "world/entities/ores/Ore.h"
#include "world/tiles/Tile.h"


class Chunk  {
    friend class ChunkManager;
    Coords cords;
    std::array<uint32_t, CHUNK_SIZE * CHUNK_SIZE> tiles;
    EntityList entities;

    public:
    explicit Chunk(const Coords &cords)
        : cords(cords), tiles(std::array<uint32_t, CHUNK_SIZE * CHUNK_SIZE>()) {

    }
    const Coords GetCoords() const {
        return cords;
    }

    const std::array<uint32_t, CHUNK_SIZE * CHUNK_SIZE>& GetTiles() const {
        return tiles;
    }

    void SetTile(const Tile& tile, const Coords& chunkCoords) {
        this->tiles[chunkCoords.y * CHUNK_SIZE + chunkCoords.x] = tile.get_tile_id();
    };

    void AddEntity(std::unique_ptr<Entity> entity) {
        entities.add(std::move(entity));
    }

    const EntityList& GetEntities() const {
        return entities;
    }

    static uint32_t GetSize() {
        return CHUNK_SIZE;
    }
    ~Chunk() = default;
};


#endif //KERNUM_CHUNK_H
