#include "ChunkGenerator.h"

#include <memory>


ChunkGenerator::ChunkGenerator(const std::shared_ptr<GeneratorConfig>& config, int seed, const std::vector<OreGenerator>& ores) : ores(ores)
 {
    this->config = config;
    this->terrainNoise = FastNoiseLite();
    this->terrainNoise.SetSeed(seed);
    this->terrainNoise.SetFrequency(0.0035);
    this->terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
}


std::unique_ptr<Chunk> ChunkGenerator::NewChunk(Coords cords) {
    auto chunk =  std::make_unique<Chunk>(cords);
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            auto tileCords = FCoords{(float)cords.x * CHUNK_SIZE + i, (float) cords.y * CHUNK_SIZE + j};
            float terrainNoiseVal = terrainNoise.GetNoise(tileCords.x, tileCords.y);
            auto terrainType = this->config->GetTerrainTypeByNoise(terrainNoiseVal);
            auto tile = this->config->GetTileByNoise(terrainType, terrainNoiseVal);
            chunk->SetTile(*tile, Coords{i, j});
            if (tile->is_solid()) {
                for (auto gen : this->ores) {
                    if (auto ore =  gen.Generate(tileCords); ore != nullptr) {
                        chunk->AddEntity(std::move(ore));
                        break;
                    }
                }
            }
        }
    }

    return chunk;
}


