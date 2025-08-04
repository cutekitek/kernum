#define TILE_SIZE 32
#include "WorldRenderer.h"

#include <cmath>
#include "imgui/imgui.h"

WorldRenderer::WorldRenderer(
    const std::shared_ptr<ChunkManager>& chunkManager,
    const std::shared_ptr<TileManager>& tileManager,
    const std::shared_ptr<TextureAtlas>& oreAtlas,
    Camera* camera):
      m_chunkManager(chunkManager),
      m_tileManager(tileManager),
      m_oreAtlas(oreAtlas),
      m_camera(camera)
{
    UpdateVisibleChunks();
    FCoords camPos = { m_camera->GetWorldX(), m_camera->GetWorldY() };
    m_lastCameraChunkPos = ChunkManager::worldCordsToChunkCoords(camPos);
}

void WorldRenderer::Update() {
    FCoords currentCamPos = {m_camera->GetWorldX(),m_camera->GetWorldY()};
    Coords currentCamChunkPos = ChunkManager::worldCordsToChunkCoords(currentCamPos);

    if (currentCamChunkPos.x != m_lastCameraChunkPos.x || currentCamChunkPos.y != m_lastCameraChunkPos.y || lastZoom != m_camera->GetZoom()) {
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
                    m_renderableChunks.insert({chunkCoords, std::make_unique<ChunkRenderer>(chunk, m_tileManager, m_camera, m_oreAtlas)});
                }
            }
        }
    }
}



void WorldRenderer::Render() {
    if (m_renderableChunks.empty()) {
        ImGui::Text("chunks loaded: 0");
        return;
    }

    Rect unloadBoundingBox = m_camera->BoundingBox();
    auto chunkSize = static_cast<float>(Chunk::GetSize());

    float buffer = chunkSize * 4;
    unloadBoundingBox.x -= buffer;
    unloadBoundingBox.y -= buffer;
    unloadBoundingBox.width += 2 * buffer;
    unloadBoundingBox.height += 2 * buffer;

    int startUnloadChunkX = static_cast<int>(std::floor(unloadBoundingBox.x / chunkSize));
    int endUnloadChunkX = static_cast<int>(std::ceil((unloadBoundingBox.x + unloadBoundingBox.width) / chunkSize));
    int startUnloadChunkY = static_cast<int>(std::floor(unloadBoundingBox.y / chunkSize));
    int endUnloadChunkY = static_cast<int>(std::ceil((unloadBoundingBox.y + unloadBoundingBox.height) / chunkSize));


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

