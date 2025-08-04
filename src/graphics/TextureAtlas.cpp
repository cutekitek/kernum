//
// Created by user on 20.07.2025.
//

#include "TextureAtlas.h"
#include <SDL3/SDL_log.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureAtlas::TextureAtlas(const std::filesystem::path& base_path, int width, int height)
    : m_width(width), m_height(height), m_basePath(base_path) {

    glGenTextures(1, &m_atlasTextureID);
    glBindTexture(GL_TEXTURE_2D, m_atlasTextureID);

    // Allocate storage for the texture on the GPU. We pass NULL for the data
    // pointer because we will fill it in piece-by-piece using glTexSubImage2D.
    glTexImage2D(
        GL_TEXTURE_2D,      // Target
        0,                  // Mipmap level
        GL_RGBA8,           // Internal format (how GPU stores it)
        m_width,            // Width
        m_height,           // Height
        0,                  // Border (must be 0)
        GL_RGBA,            // Format of pixel data (we'll upload RGBA)
        GL_UNSIGNED_BYTE,   // Type of pixel data
        nullptr             // Pointer to data (none for now)
    );

    // Set texture parameters. GL_NEAREST is common for pixel art / atlases.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Unbind texture when done
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Created OpenGL Texture Atlas with ID: %u (%dx%d)", m_atlasTextureID, m_width, m_height);
}

TextureAtlas::~TextureAtlas() {
    if (m_atlasTextureID != 0) {
        glDeleteTextures(1, &m_atlasTextureID);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroyed OpenGL Texture Atlas with ID: %u", m_atlasTextureID);
    }
}

bool TextureAtlas::find_space(int w, int h, SDL_Rect* outRect) {
    // Check if it fits on the current row
    if (m_nextX + w > m_width) {
        // Doesn't fit, move to the next row
        m_nextY += m_rowMaxHeight;
        m_nextX = 0;
        m_rowMaxHeight = 0;
    }

    // Check if it fits on the new row (and in the atlas at all)
    if (m_nextY + h > m_height) {
        return false; // Not enough space in the atlas
    }

    // It fits!
    outRect->x = m_nextX;
    outRect->y = m_nextY;
    outRect->w = w;
    outRect->h = h;

    // Advance the cursor for the next texture
    m_nextX += w;
    if (h > m_rowMaxHeight) {
        m_rowMaxHeight = h;
    }
    return true;
}

const Texture* TextureAtlas::load_texture(const std::string& fileName) {
    // 1. Check cache
    auto it = m_regions.find(fileName);
    if (it != m_regions.end()) {
        return it->second.get();
    }

    // 2. Load image from disk to CPU memory using stb_image
    auto fullPath = (m_basePath / fileName).string();
    int width, height, channels;
    // Force 4 channels (RGBA) for consistency
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!data) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture file '%s'! STB_image Error", fullPath.c_str());
        return nullptr;
    }

    // 3. Find space in our atlas
    SDL_Rect destRect;
    if (!find_space(width, height, &destRect)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture atlas is full! Cannot load '%s'", fullPath.c_str());
        stbi_image_free(data);
        return nullptr;
    }

    // 4. Upload the pixel data to the correct sub-region of the GPU texture
    // This is the OpenGL equivalent of the SDL_gpu copy command sequence.
    glBindTexture(GL_TEXTURE_2D, m_atlasTextureID);
    glTexSubImage2D(
        GL_TEXTURE_2D,      // Target
        0,                  // Mipmap level
        destRect.x,         // X offset in the destination texture
        destRect.y,         // Y offset in the destination texture
        destRect.w,         // Width of the sub-image
        destRect.h,         // Height of the sub-image
        GL_RGBA,            // Format of the source pixel data
        GL_UNSIGNED_BYTE,   // Type of the source pixel data
        data                // Pointer to the source pixel data
    );
    glBindTexture(GL_TEXTURE_2D, 0);

    // 5. Clean up CPU-side image data
    stbi_image_free(data);

    // 6. Create the Texture region info and cache it
    auto textureRegion = std::make_unique<Texture>(m_atlasTextureID, destRect, m_width, m_height);
    const Texture* result = textureRegion.get();
    m_regions[fileName] = std::move(textureRegion);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loaded '%s' into atlas at {%d, %d, %d, %d}", fileName.c_str(), destRect.x, destRect.y, destRect.w, destRect.h);

    return result;
}