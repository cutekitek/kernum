//
// Created by user on 25.07.2025.
//

#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H



#include <unordered_map>
#include <vector>
#include <memory>
#include <unordered_set>

#include "render/base/Camera.h"
#include "render/base/TileRenderer.h"
#include "world/chunks/ChunkManager.h"
#include "utils/FCoords.h"


class WorldRenderer {
public:
    WorldRenderer(
    const std::shared_ptr<Shader> &shader,
    const std::shared_ptr<ChunkManager>& chunkManager,
    const std::shared_ptr<TileManager>& tileManager,
    Camera* camera
    );

    // Checks for camera movement and updates visible chunks
    void Update();

    // Renders all visible chunks
    void Render();

private:
    // Generates renderable TileInfo from a raw Chunk object
    std::vector<TileInfo> GenerateTileInfoForChunk(const Chunk* chunk);

    // The main logic for loading/unloading chunks based on camera position
    void UpdateVisibleChunks();

    std::shared_ptr<ChunkManager> m_chunkManager;
    std::shared_ptr<TileManager> m_tileManager;
    Camera* m_camera;

    // The core data structure: maps chunk coordinates to its renderable tile data
    std::unordered_map<Coords, std::unique_ptr<TileRenderer>> m_renderableChunks;
    std::shared_ptr<Shader> m_shader;

    Coords m_lastCameraChunkPos = {0, 0};
    float lastZoom = 1.0f;
};

#endif