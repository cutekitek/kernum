
#include "TileManager.h"

#include <filesystem>
#include <memory>

std::shared_ptr<Tile> TileManager::CreateTile(const std::string& name, const std::string& textureName, bool solid) {
    uint32_t tileId = this->tiles.size();

    auto texture = textureManager->LoadTexture((baseTexturePath / textureName).string());
    if (texture == nullptr) {
        return nullptr;
    }

    auto tile = std::make_shared<Tile>(tileId, name, texture, solid);
    this->tiles.push_back(tile);
    this->tilesMap[name] = tile;
    return tile;
}

const std::vector<std::shared_ptr<Tile>> & TileManager::GetTiles() {
    return this->tiles;
}

std::shared_ptr<Tile> TileManager::GetTileById(uint32_t tileId) {
    return this->tiles[tileId];
}

std::shared_ptr<Tile> TileManager::GetTileByName(const std::string &name) {
    return this->tilesMap.find(name)->second;
}

