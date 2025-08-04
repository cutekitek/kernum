
#include "TileManager.h"
#include "Tile.h"

#include <filesystem>
#include <functional>
#include <memory>
#include <optional>

const Tile* TileManager::CreateTile(const std::string& name, bool solid, const std::vector<std::string>& variants) {
    uint32_t tileId = this->tiles.size();
    auto textures = std::vector<const Texture*>();
    for (auto& filename : variants) {
        auto text = textureAtlas->load_texture((baseTexturePath / filename).string());
        if (text == nullptr) {
            return nullptr;
        }
        textures.push_back(text);
    }


    auto tile = Tile(tileId, name, textures, solid);
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

const TextureAtlas * TileManager::GetTextureAtlas() const {
    return textureAtlas.get();
}

const Tile* TileManager::GetTileByName(const std::string &name) {
    auto tileId =  this->tilesMap.find(name);
    if (tileId == this->tilesMap.end()) {
        return nullptr;
    }
    return &this->tiles[tileId->second];
}
