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
#include "../../graphics/TextureAtlas.h"


class TileManager {
    const std::filesystem::path baseTexturePath = "tiles";
    std::vector<Tile> tiles;
    std::unordered_map<std::string, int> tilesMap;
    std::unique_ptr<TextureAtlas> textureAtlas;

public:
    explicit TileManager()
    {
        textureAtlas = std::make_unique<TextureAtlas>("resources/textures", 1024, 1024);
    }

    ~TileManager() = default;

    const Tile* CreateTile(const std::string &name, const std::string &textureName, bool solid);

    const std::vector<Tile> & GetTiles();

    const Tile* GetTileByName(const std::string &name);

    const Tile& GetTileById(uint32_t tileId);

    const TextureAtlas* GetTextureAtlas() const;
};


#endif //TILEMANAGER_H
