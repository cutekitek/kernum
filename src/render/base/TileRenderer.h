#pragma once
#include <memory>

#include "Shader.h"
#include "SDL3/SDL_render.h"
#include "../../world/chunks/ChunkManager.h"
#include <glm/trigonometric.hpp>
#include "./Camera.h"

struct TileInfo {
    glm::vec4 pos;
    glm::vec4 uv;
};

class TileRenderer {
    GLfloat quadVertices[12] = { 0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f};


    std::shared_ptr<Shader> shader;
    std::shared_ptr<TileManager> tileManager;
    Camera* camera;

    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
    GLuint instanceVbo;
    int tilesCount;

    public:
    TileRenderer(const std::shared_ptr<Shader> &shader, const std::shared_ptr<TileManager> &tile_manager,  Camera *camera, const std::vector<TileInfo>& tiles)
        : shader(shader),
          tileManager(tile_manager),camera(camera),tilesCount(tiles.size()) {
        init(tiles);
    }

    ~TileRenderer();

    void Render() const {
        shader->Bind();
        auto proj = camera->GetProjectionMatrix();
        shader->SetMat4("projection", proj);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tileManager->GetTextureAtlas()->get_atlas_id());
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, tilesCount);
        glBindVertexArray(0);
        shader->Unbind();
    }

    void SetTiles(const std::vector<TileInfo>& tiles) {
        tilesCount = tiles.size();
        glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
        glBufferData(GL_ARRAY_BUFFER, tiles.size() * sizeof(TileInfo), tiles.data(), GL_DYNAMIC_DRAW);
    }

    private:
    void init(const std::vector<TileInfo>& tiles);
};


