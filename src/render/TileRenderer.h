#pragma once
#include <memory>

#include "Shader.h"
#include "SDL3/SDL_render.h"
#include "../world/chunks/ChunkManager.h"

class TileRenderer {
    GLfloat quadVertices[12] = { 0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f}; // bottom right corner


    std::shared_ptr<Shader> shader;
    std::shared_ptr<TileManager> tileManager;
    uint32_t rows, cols;
    float tileSize;
    float projection[16];
    uint32_t tilesCount;

    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
    GLuint instanceVbo;

    public:
    TileRenderer(const std::shared_ptr<Shader> &shader, const std::shared_ptr<TileManager> &tile_manager, float tile_size, SDL_Window* window)
        : shader(shader),
          tileManager(tile_manager),tileSize(tile_size) {

        int ww,wh;
        SDL_GetWindowSize(window, &ww, &wh);
        SetWindowSize(ww, wh);
        init();
    }

    ~TileRenderer() = default;

    void dispatchEvent(SDL_Event *event);
    void SetWindowSize(int width, int height);
    void Render();
    void SetTileMap(const std::vector<uint32_t> &tiles);

    private:
    void init();
    void updateTileMap(const std::vector<uint32_t> &tiles);
};


