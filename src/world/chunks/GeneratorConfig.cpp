
#include "GeneratorConfig.h"


GeneratorConfig::~GeneratorConfig() {
}

TerrainType GeneratorConfig::GetTerrainTypeByNoise(float noise) {
    if (noise >= -0.8) {
        return TerrainType::SURFACE;
    }
    return TerrainType::OCEAN;
}

std::shared_ptr<Tile> GeneratorConfig::GetTileByNoise(TerrainType type, float noise) {
    switch (type) {
        case TerrainType::SURFACE:
            return this->tileManager->GetTileByName("grass");
        case TerrainType::OCEAN:
            return  this->tileManager->GetTileByName("water");
        default:
            return this->tileManager->GetTileByName("grass");
    }
}
