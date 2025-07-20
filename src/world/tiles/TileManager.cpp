
#include "TileManager.h"
#include "Tile.h"

#include <filesystem>
#include <functional>
#include <memory>
#include <optional>

const Tile* TileManager::CreateTile(const std::string& name, const std::string& textureName, bool solid) {
    uint32_t tileId = this->tiles.size();

    auto texture = textureManager->LoadTexture((baseTexturePath / textureName).string());
    if (texture == nullptr) {
        return nullptr;
    }

    auto tile = Tile(tileId, name, *texture, solid);
    this->tiles.push_back(tile);
    this->tilesMap[name] = tileId;
    return &this->tiles[tileId];
}

const std::vector<Tile> & TileManager::GetTiles() {
    return this->tiles;
}

const Tile& TileManager::GetTileById(uint32_t tileId) {
    return this->tiles[tileId];
}

const Tile* TileManager::GetTileByName(const std::string &name) {
    auto tileId =  this->tilesMap.find(name);
    if (tileId == this->tilesMap.end()) {
        return nullptr;
    }
    return &this->tiles[tileId->second];
}
