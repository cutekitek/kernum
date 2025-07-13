//
// Created by user on 13.07.2025.
//

#ifndef TILEMANAGER_H
#define TILEMANAGER_H
#include <cstdint>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Tile.h"
#include "../../graphics/TextureManager.h"


class TileManager {
    const std::filesystem::path baseTexturePath = "tiles";
    std::vector<std::shared_ptr<Tile> > tiles;
    std::unordered_map<std::string, std::shared_ptr<Tile> > tilesMap;
    std::shared_ptr<TextureManager> textureManager;

public:
    explicit TileManager(const std::shared_ptr<TextureManager> &texture_manager)
        : tiles(),
          tilesMap(),
          textureManager(texture_manager) {
    }

    ~TileManager() = default;

    std::shared_ptr<Tile> CreateTile(const std::string &name, const std::string &textureName, bool solid);

    const std::vector<std::shared_ptr<Tile> > &GetTiles();

    std::shared_ptr<Tile> GetTileByName(const std::string &name);

    std::shared_ptr<Tile> GetTileById(uint32_t tileId);
};


#endif //TILEMANAGER_H
