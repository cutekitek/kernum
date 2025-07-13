#ifndef KERNUM_CHUNK_H
#define KERNUM_CHUNK_H
#define CHUNK_SIZE 64

#include <cstdint>

#include "../../utils/FCoords.h"


class Chunk  {
    friend class ChunkManager;
    Coords cords;
    std::array<uint32_t, CHUNK_SIZE * CHUNK_SIZE> tiles;

    public:
    explicit Chunk(const Coords &cords, const std::array<uint32_t, CHUNK_SIZE * CHUNK_SIZE> &tiles)
        : cords(cords), tiles(tiles) {
    }

    ~Chunk() = default;
};


#endif //KERNUM_CHUNK_H
