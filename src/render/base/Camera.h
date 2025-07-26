#pragma once
#define TILE_SIZE 32

#include <SDL3/SDL.h>

#include "glm/glm.hpp"
#include <glm/ext/matrix_clip_space.hpp>

#include "../../utils/Rect.h"
#include "glm/gtc/type_ptr.hpp"
#include "utils/FCoords.h"


struct Vector2D {
    double x, y;
};

class Camera {
public:

    Camera(float startX, float startY, float screenWidth, float screenHeight, float initialZoom = 1.0f);
    void DispatchEvent(const SDL_Event* event);
    void Update();

    Vector2D ScreenToWorld(int screenX, int screenY, SDL_Renderer* renderer) const;

    Vector2D GetWorldPosition() const { return {m_worldX, m_worldY}; }
    float GetWorldX() const { return m_worldX; }
    float GetWorldY() const { return m_worldY; }
    int GetScreenWidth() const {return screenWidth; }
    int GetScreenHeight() const {return screenHeight; }
    float GetZoom() const { return m_zoom; }

    glm::mat4 GetProjectionMatrix() const {
        return m_proj;
    };

    Rect BoundingBox() const {
       return boundingBox;
    }
    FCoords ScreenToWorld(int screenX, int screenY) const {
        return FCoords(m_worldX + screenX / m_zoom, m_worldY + screenY / m_zoom);
    }



private:
    // Actual camera position
    float m_worldX, m_worldY;
    
    // Target position for smooth movement
    float m_targetWorldX, m_targetWorldY;

    float m_zoom;
    float m_lerpSpeed; // Speed of interpolation

    // For panning logic
    bool m_isDragging;
    float m_prevMouseX, m_prevMouseY;
    float screenWidth, screenHeight;

    glm::mat4 m_proj;
    Rect boundingBox;

    void updateAfterMove();
};