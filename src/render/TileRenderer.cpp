#include "TileRenderer.h"
#define TILES_HOR 240


void TileRenderer::dispatchEvent(SDL_Event *event) {}

void TileRenderer::SetWindowSize(int width, int height) {
}

void TileRenderer::Render() {
    float left = 0.0f;
    float right = 1920;
    float bottom = 1080; // TODO
    float top = 0.0f;
    float near = -1.0f;
    float far = 1.0f;

    // Note: OpenGL matrices are column-major
    float projection[16] = {
        2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (far - near), 0.0f,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f
    };

    shader->Bind();
    shader->SetFloat("tileSize", 32.0);
    shader->SetInt("cols", 120);
    shader->SetMat4("projection", projection);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tileManager->GetTextureAtlas()->get_atlas_id());

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, tilesCount);
    glBindVertexArray(0);
}

void TileRenderer::SetTileMap(const std::vector<uint32_t> &tiles) {
    std::vector<Texture::UVs> tileUVs = std::vector<Texture::UVs>(tiles.size());
    for (int i = 0; i < tiles.size(); i++) {
        tileUVs[i] = tileManager->GetTileById(tiles[i]).get_texture().get_uvs();
    }
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    glBufferData(GL_ARRAY_BUFFER, tileUVs.size() * sizeof(Texture::UVs), tileUVs.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    tilesCount = tileUVs.size();
}

void TileRenderer::init() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &instanceVbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    // We don't load data yet, just allocate memory. GL_DYNAMIC_DRAW means we'll update it often.
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Texture::UVs), (void*)0);
    glVertexAttribDivisor(1, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TileRenderer::updateTileMap(const std::vector<uint32_t> &tiles) {

}
