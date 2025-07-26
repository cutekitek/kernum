//
// Created by user on 25.07.2025.
//
#define TILE_SIZE 32
#include "WorldRenderer.h"

#include <cmath>
#include "imgui/imgui.h"

WorldRenderer::WorldRenderer(
    const std::shared_ptr<Shader> &shader,
    const std::shared_ptr<ChunkManager>& chunkManager,
    const std::shared_ptr<TileManager>& tileManager,
    Camera* camera):
      m_chunkManager(chunkManager),
      m_tileManager(tileManager),
      m_camera(camera), m_shader(shader)
{
    UpdateVisibleChunks();
    FCoords camPos = { (float)m_camera->GetWorldX(), (float)m_camera->GetWorldY() };
    m_lastCameraChunkPos = ChunkManager::worldCordsToChunkCoords(camPos);
}

void WorldRenderer::Update() {
    FCoords currentCamPos = {(float)m_camera->GetWorldX(), (float)m_camera->GetWorldY()};
    Coords currentCamChunkPos = ChunkManager::worldCordsToChunkCoords(currentCamPos);

    if (currentCamChunkPos.x != m_lastCameraChunkPos.x || currentCamChunkPos.y != m_lastCameraChunkPos.y || lastZoom != m_camera->GetZoom()) {
        std::cout << "Camera moved to a new chunk, updating visible set." << std::endl;
        UpdateVisibleChunks();
        m_lastCameraChunkPos = currentCamChunkPos;
        lastZoom = m_camera->GetZoom();
    }
}


void WorldRenderer::UpdateVisibleChunks() {
    // ...
    Rect boundingBox = m_camera->BoundingBox();
    float chunkSize = m_chunkManager->GetChunkSize();
    boundingBox.x -= chunkSize*4;
    boundingBox.y -= chunkSize*4;
    boundingBox.width += 8 * chunkSize;
    boundingBox.height += 8 * chunkSize;
    int startChunkX = static_cast<int>(std::floor(boundingBox.x / chunkSize));
    int endChunkX = static_cast<int>(std::ceil((boundingBox.x + boundingBox.width) / chunkSize));
    int startChunkY = static_cast<int>(std::floor(boundingBox.y / chunkSize));
    int endChunkY = static_cast<int>(std::ceil((boundingBox.y + boundingBox.height) / chunkSize));

    for (int cy = startChunkY; cy < endChunkY; ++cy) {
        for (int cx = startChunkX; cx < endChunkX; ++cx) {
            Coords chunkCoords = {cx, cy};
            if (!m_renderableChunks.contains(chunkCoords)) {
                auto chunk = m_chunkManager->LoadChunk(chunkCoords);
                if (chunk) {
                    auto tileMap = GenerateTileInfoForChunk(chunk);
                    auto renderer = std::make_unique<TileRenderer>(m_shader, m_tileManager, m_camera, tileMap);
                    m_renderableChunks.insert({chunkCoords, std::move(renderer)});
                }
            }
        }
    }
}


std::vector<TileInfo> WorldRenderer::GenerateTileInfoForChunk(const Chunk* chunk) {
    std::vector<TileInfo> tiles;
    if (!chunk) return tiles;

    const uint32_t chunkSize = m_chunkManager->GetChunkSize();
    tiles.reserve(chunkSize * chunkSize);

    Coords chunkCoords = chunk->GetCoords();
    float chunkWorldX = chunkCoords.x * static_cast<float>(chunkSize);
    float chunkWorldY = chunkCoords.y * static_cast<float>(chunkSize);

    auto tileIDs = chunk->GetTiles();
    for (uint32_t y = 0; y < chunkSize; ++y) {
        for (uint32_t x = 0; x < chunkSize; ++x) {
            uint16_t tileId = tileIDs[y * chunkSize + x];


            TileInfo info;
            info.pos = glm::vec4(chunkWorldX + static_cast<float>(x), chunkWorldY + static_cast<float>(y), 1.0f, 1.0f);

            info.uv = m_tileManager->GetTileById(tileId).get_texture().get_uvs();

            tiles.push_back(info);
        }
    }

    return tiles;
}


void WorldRenderer::Render() {
    if (m_renderableChunks.empty()) {
        ImGui::Text("chunks loaded: 0");
        return;
    }

    Rect unloadBoundingBox = m_camera->BoundingBox();
    float chunkSize = m_chunkManager->GetChunkSize();

    float buffer = chunkSize * 4;
    unloadBoundingBox.x -= buffer;
    unloadBoundingBox.y -= buffer;
    unloadBoundingBox.width += 2 * buffer;
    unloadBoundingBox.height += 2 * buffer;

    int startUnloadChunkX = static_cast<int>(floor(unloadBoundingBox.x / chunkSize));
    int endUnloadChunkX = static_cast<int>(ceil((unloadBoundingBox.x + unloadBoundingBox.width) / chunkSize));
    int startUnloadChunkY = static_cast<int>(floor(unloadBoundingBox.y / chunkSize));
    int endUnloadChunkY = static_cast<int>(ceil((unloadBoundingBox.y + unloadBoundingBox.height) / chunkSize));

    // --- End of Fix ---

    for (auto it = m_renderableChunks.begin(); it != m_renderableChunks.end(); ) {
        const Coords& chunkCoords = it->first;

        bool isOutside = chunkCoords.x < startUnloadChunkX ||
                         chunkCoords.x >= endUnloadChunkX ||
                         chunkCoords.y < startUnloadChunkY ||
                         chunkCoords.y >= endUnloadChunkY;

        if (isOutside) {
            it = m_renderableChunks.erase(it);
        } else {
            it->second->Render();
            ++it;
        }
    }
}

