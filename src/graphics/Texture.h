#pragma once

#include <string>

#include <SDL3/SDL.h>

#include "glad/glad.h"

class Texture {
public:
    // UV coordinates: [minU, minV, maxU, maxV]
    struct UVs {
        float u1, v1; // Top-left
        float u2, v2; // Bottom-right
    };

private:
    GLuint m_parentAtlasID;
    UVs m_uvs;
    int m_width;
    int m_height;

public:
    // Calculates and stores normalized UVs from pixel coordinates.
    Texture(GLuint parentID, const SDL_Rect& pixelRect, int atlasWidth, int atlasHeight)
        : m_parentAtlasID(parentID), m_width(pixelRect.w), m_height(pixelRect.h)
    {
        // Convert pixel coordinates to normalized (0.0 to 1.0) texture coordinates.
        m_uvs.u1 = static_cast<float>(pixelRect.x) / atlasWidth;
        m_uvs.v1 = static_cast<float>(pixelRect.y) / atlasHeight;
        m_uvs.u2 = static_cast<float>(pixelRect.x + pixelRect.w) / atlasWidth;
        m_uvs.v2 = static_cast<float>(pixelRect.y + pixelRect.h) / atlasHeight;
    }

    ~Texture() = default;

    // Get the ID of the parent OpenGL texture (the whole atlas)
    [[nodiscard]] GLuint get_parent_atlas_id() const {
        return m_parentAtlasID;
    }

    // Get the pre-calculated UV coordinates for this specific region.
    [[nodiscard]] const UVs& get_uvs() const {
        return m_uvs;
    }

    // Get original dimensions in pixels
    [[nodiscard]] int get_width() const { return m_width; }
    [[nodiscard]] int get_height() const { return m_height; }
};
