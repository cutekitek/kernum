#include "ChunkManager.h"


std::shared_ptr<Chunk> ChunkManager::LoadChunk(Coords chunkCoords) {
    auto chunk = this->chunks[chunkCoords];
    if (chunk == nullptr) {
        chunk = generator->NewChunk(chunkCoords);
        this->chunks[chunkCoords] = chunk;
    }
    return chunk;
}

std::shared_ptr<Tile> ChunkManager::GetTileByCoords(FCoords worldCoords) {
    auto chunkCoords = worldCordsToChunkCoords(worldCoords);
    auto chunk = LoadChunk(chunkCoords);
    auto tileId =  chunk->tiles[((int)worldCoords.y % CHUNK_SIZE) * CHUNK_SIZE + (int)worldCoords.x % CHUNK_SIZE];
    return  tileManager->GetTileById(tileId);
}

Coords ChunkManager::worldCordsToChunkCoords(FCoords cords) {
    return  Coords{((int)cords.x / CHUNK_SIZE + (int)cords.x % CHUNK_SIZE), ((int)cords.y / CHUNK_SIZE + (int)cords.y % CHUNK_SIZE)};
}
