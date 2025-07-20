#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Chunk.h"
#include "ChunkGenerator.h"

#include "../../utils/FCoords.h"



class ChunkManager {
    std::unordered_map<Coords, std::unique_ptr<Chunk>> chunks;
    std::shared_ptr<ChunkGenerator> generator;
    std::shared_ptr<TileManager> tileManager;
    public:
    ChunkManager(const std::shared_ptr<ChunkGenerator> &generator, const std::shared_ptr<TileManager> &tile_manager)
        : chunks(),
          generator(generator),
          tileManager(tile_manager) {
    }

    const Chunk* LoadChunk(FCoords worldCords);
    inline uint32_t GetChunkSize() {return CHUNK_SIZE;};

private:
    static inline Coords worldCordsToChunkCoords(FCoords cords);
};


#endif
