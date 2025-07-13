//
// Created by user on 13.07.2025.
//

#ifndef GENERATORCONFIG_H
#define GENERATORCONFIG_H
#include "../tiles/Tile.h"
#include "../tiles/TileManager.h"

enum class TerrainType{
    SURFACE,
    OCEAN
};


class GeneratorConfig {
    std::shared_ptr<TileManager> tileManager;
    public:

    explicit GeneratorConfig(const std::shared_ptr<TileManager> &tile_manager)
        : tileManager(tile_manager) {}
    ~GeneratorConfig();

    static TerrainType GetTerrainTypeByNoise(float noise);
    std::shared_ptr<Tile> GetTileByNoise(TerrainType type, float noise);

};



#endif
