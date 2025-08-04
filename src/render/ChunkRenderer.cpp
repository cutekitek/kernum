//
// Created by user on 02.08.2025.
//

#include "ChunkRenderer.h"

void ChunkRenderer::initTerrainRenderer(const std::shared_ptr<TileManager>& tileManager) {
    std::vector<TileInfo> tiles;

    const uint32_t chunkSize = Chunk::GetSize();
    tiles.reserve(chunkSize * chunkSize);

    Coords chunkCoords = chunk->GetCoords();
    float chunkWorldX = chunkCoords.x * static_cast<float>(chunkSize);
    float chunkWorldY = chunkCoords.y * static_cast<float>(chunkSize);

    auto tileIDs = chunk->GetTiles();
    for (uint32_t y = 0; y < chunkSize; ++y) {
        for (uint32_t x = 0; x < chunkSize; ++x) {
            uint16_t tileId = tileIDs[y * chunkSize + x];
            auto variants = tileManager->GetTileById(tileId).get_texture_variants();
            float tilePosX = chunkWorldX + static_cast<float>(x);
            float tilePosY = chunkWorldY + static_cast<float>(y);

            TileInfo info;
            info.pos = glm::vec4(tilePosX, tilePosY, 1.0f, 1.0f);
            auto variantId = (static_cast<int64_t>(tilePosX) * 0x9e3779b97f4a7c15  ^ static_cast<int64_t>(tilePosY)) % variants.size();
            info.uv = variants[variantId]->get_uvs();
            tiles.push_back(info);
        }
    }

    this->terrainRenderer = std::make_unique<TileRenderer>(tileManager->GetTextureAtlas(), camera, tiles);
}

void ChunkRenderer::initOreRenderer(const std::shared_ptr<TileManager>& tileManager) {
    std::vector<TileInfo> tiles;
    for (auto ore: chunk->GetEntities().iter<Ore>()) {
        auto pos = ore->GetPosition();
        auto text = ore->GetTexture();
        tiles.push_back(TileInfo{glm::vec4(pos.x, pos.y, 1.0f, 1.0f), text->get_uvs()});
    }
    if (!tiles.empty()) {
        this->oreRenderer = std::make_unique<TileRenderer>(oreAtlas.get(), camera, tiles);
    }
}

void ChunkRenderer::Render() {
    this->terrainRenderer->Render();
    if (this->oreRenderer) {
        this->oreRenderer->Render();
    }
}
