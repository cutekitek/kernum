//
// Created by user on 13.07.2025.
//

#ifndef TILEMANAGER_H
#define TILEMANAGER_H
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Tile.h"
#include "../../graphics/TextureManager.h"


class TileManager {
    const std::filesystem::path baseTexturePath = "tiles";
    std::vector<Tile> tiles;
    std::unordered_map<std::string, int> tilesMap;
    std::shared_ptr<TextureManager> textureManager;

public:
    explicit TileManager(const std::shared_ptr<TextureManager> &texture_manager)
        : tiles(),
          tilesMap(),
          textureManager(texture_manager) {
    }

    ~TileManager() = default;

    const Tile* CreateTile(const std::string &name, const std::string &textureName, bool solid);

    const std::vector<Tile> & GetTiles();

    const Tile* GetTileByName(const std::string &name);

    const Tile& GetTileById(uint32_t tileId);
};


#endif //TILEMANAGER_H
