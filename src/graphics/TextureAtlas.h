//
// Created by user on 20.07.2025.
//

#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Texture.h"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <../include/glad/glad.h>

class TextureAtlas {
private:
    GLuint m_atlasTextureID = 0;
    int m_width;
    int m_height;
    std::filesystem::path m_basePath;
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_regions;

    // Simple packing logic state (Shelf Packer)
    int m_nextX = 0;
    int m_nextY = 0;
    int m_rowMaxHeight = 0;

    // Private helper to find space for a new texture.
    bool find_space(int w, int h, SDL_Rect* outRect);

public:
    TextureAtlas(const std::filesystem::path& base_path, int width, int height);
    ~TextureAtlas();

    // Prevent copying
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;

    // Loads an image and packs it into this specific atlas.
    const Texture* load_texture(const std::string& fileName);

    // Getter for the underlying OpenGL texture ID.
    [[nodiscard]] GLuint get_texture_id() const {
        return m_atlasTextureID;
    }

    void Bind() {
        glBindTexture(GL_TEXTURE_2D, m_atlasTextureID);
    }
};


#endif //TEXTUREATLAS_H
