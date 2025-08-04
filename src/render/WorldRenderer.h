//
// Created by user on 25.07.2025.
//

#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H



#include <unordered_map>
#include <memory>

#include "ChunkRenderer.h"
#include "render/base/Camera.h"
#include "world/chunks/ChunkManager.h"
#include "utils/FCoords.h"


class WorldRenderer {
public:
    WorldRenderer(
    const std::shared_ptr<ChunkManager>& chunkManager,
    const std::shared_ptr<TileManager>& tileManager,
    const std::shared_ptr<TextureAtlas>& oreAtlas,
    Camera* camera
    );

    void Update();
    void Render();

private:
    void UpdateVisibleChunks();

    std::shared_ptr<ChunkManager> m_chunkManager;
    std::shared_ptr<TileManager> m_tileManager;
    std::shared_ptr<TextureAtlas> m_oreAtlas;
    Camera* m_camera;

    std::unordered_map<Coords, std::unique_ptr<ChunkRenderer>> m_renderableChunks;
    std::shared_ptr<Shader> m_shader;

    Coords m_lastCameraChunkPos = {0, 0};
    float lastZoom = 1.0f;
};

#endif