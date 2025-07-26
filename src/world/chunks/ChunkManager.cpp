#include "ChunkManager.h"

#include <iostream>


const Chunk* ChunkManager::LoadChunk(FCoords worldCords) {
    auto chunkCoords = worldCordsToChunkCoords(worldCords);
    return ChunkManager::LoadChunk(chunkCoords);
}

const Chunk* ChunkManager::LoadChunk(Coords chunkCoords) {
    auto chunk = this->chunks[chunkCoords].get();
    if (chunk == nullptr) {
        auto generatedChunk = generator->NewChunk(chunkCoords);
        this->chunks[chunkCoords] = std::move(generatedChunk);
        std::cout << "Chunk created at" << chunkCoords.x << " " << chunkCoords.y << std::endl;
        return this->chunks[chunkCoords].get();
    }

    return chunk;
}
