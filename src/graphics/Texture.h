#pragma once

#include <string>

#include <SDL3/SDL.h>

#include "glad/glad.h"
#include "glm/glm.hpp"

class Texture {

private:
    GLuint m_parentAtlasID;
    glm::vec4 m_uvs;
    int m_width;
    int m_height;

public:
    // Calculates and stores normalized UVs from pixel coordinates.
    Texture(GLuint parentID, const SDL_Rect& pixelRect, int atlasWidth, int atlasHeight)
        : m_parentAtlasID(parentID), m_width(pixelRect.w), m_height(pixelRect.h)
    {
        m_uvs = glm::vec4(static_cast<float>(pixelRect.x) / atlasWidth,
                        static_cast<float>(pixelRect.y) / atlasHeight,
                        static_cast<float>(pixelRect.x + pixelRect.w) / atlasWidth,
                        static_cast<float>(pixelRect.y + pixelRect.h) / atlasHeight);

    }

    ~Texture() = default;

    // Get the ID of the parent OpenGL texture (the whole atlas)
    [[nodiscard]] GLuint get_parent_atlas_id() const {
        return m_parentAtlasID;
    }

    // Get the pre-calculated UV coordinates for this specific region.
    [[nodiscard]] const glm::vec4& get_uvs() const {
        return m_uvs;
    }

    // Get original dimensions in pixels
    [[nodiscard]] int get_width() const { return m_width; }
    [[nodiscard]] int get_height() const { return m_height; }
};
