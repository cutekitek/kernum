//
// Created by user on 02.08.2025.
//

#ifndef OREGENERATOR_H
#define OREGENERATOR_H
#include <optional>

#include "noise.h"
#include "graphics/Texture.h"
#include "world/entities/ores/Ore.h"


class OreGenerator {
    int minCount;
    int maxCount;
    float noiseTreshold;
    const Texture *texture;
    FastNoiseLite noiseGen;
    public:
        OreGenerator(int minCount, int maxCount, int seed, float rarity, const Texture *texture): minCount(minCount), maxCount(maxCount), texture(texture) {
            this->noiseGen = FastNoiseLite();
            this->noiseGen.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
            this->noiseGen.SetSeed(seed);
            this->noiseGen.SetFrequency(0.02);
            this->noiseTreshold = rarity;
        };

        std::unique_ptr<Ore> Generate(FCoords coords) {
            float noise = (this->noiseGen.GetNoise(coords.x, coords.y)  + 1.0f) / 2;
            if (noise > this->noiseTreshold) {
                return nullptr;
            }
            return  std::make_unique<Ore>(texture, 100, coords);
        }

};



#endif //OREGENERATOR_H
