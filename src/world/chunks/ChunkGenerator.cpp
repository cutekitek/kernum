#include "ChunkGenerator.h"

#include <iostream>


ChunkGenerator::ChunkGenerator(const std::shared_ptr<GeneratorConfig>& config, const int seed)
 {
    this->config = config;
    this->terrainNoise = FastNoiseLite();
    this->terrainNoise.SetSeed(seed);
    this->terrainNoise.SetFrequency(0.002);
    this->terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
}


std::shared_ptr<Chunk> ChunkGenerator::NewChunk(Coords cords) {
    std::array<uint32_t, CHUNK_SIZE * CHUNK_SIZE> tiles{};
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            float terrainNoiseVal = terrainNoise.GetNoise((float)cords.x * CHUNK_SIZE + i, (float) cords.y * CHUNK_SIZE + j);
            auto terrainType = this->config->GetTerrainTypeByNoise(terrainNoiseVal);
            auto tile = this->config->GetTileByNoise(terrainType, terrainNoiseVal);
            tiles[j * CHUNK_SIZE + i] = tile->get_tile_id();
        }
    }

    return std::make_shared<Chunk>(cords, tiles);
}


