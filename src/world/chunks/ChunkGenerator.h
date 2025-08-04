#ifndef CHUNKGENERATOR_H
#define CHUNKGENERATOR_H

#include <memory>

#include "Chunk.h"
#include "GeneratorConfig.h"
#include "noise.h"
#include "OreGenerator.h"
#include "../../utils/FCoords.h"

class ChunkGenerator {
    FastNoiseLite terrainNoise;
    std::shared_ptr<GeneratorConfig> config;
    const std::vector<OreGenerator> ores;

    public:
    ChunkGenerator(const std::shared_ptr<GeneratorConfig>& config, int seed, const std::vector<OreGenerator>& ores);
    ~ChunkGenerator() = default;
    std::unique_ptr<Chunk> NewChunk(Coords cords);

};



#endif //CHUNKGENERATOR_H
