#ifndef CHUNKGENERATOR_H
#define CHUNKGENERATOR_H
#include <cstdint>
#include <memory>

#include "Chunk.h"
#include "GeneratorConfig.h"
#include "noise.h"
#include "../../utils/FCoords.h"

class ChunkGenerator {
    FastNoiseLite terrainNoise;
    std::shared_ptr<GeneratorConfig> config;
    public:
    ChunkGenerator(const std::shared_ptr<GeneratorConfig>& config, const int seed);
    ~ChunkGenerator() = default;
    std::shared_ptr<Chunk> NewChunk(Coords cords);

};



#endif //CHUNKGENERATOR_H
