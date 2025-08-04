#include "Camera.h"
#include <algorithm>
#include <cmath>


Camera::Camera(float startX, float startY, float screenWidth, float screenHeight, float initialZoom)
    : m_worldX(startX),
      m_worldY(startY),
      m_targetWorldX(startX),
      m_targetWorldY(startY),
      m_zoom(initialZoom),
      m_lerpSpeed(0.1f),
      m_isDragging(false),
      m_prevMouseX(0),
      m_prevMouseY(0),
screenWidth(screenWidth), screenHeight(screenHeight) {
    updateAfterMove();
}

void Camera::Update() {
}

void Camera::updateAfterMove() {
    float viewWidthInTiles = (screenWidth / TILE_SIZE) / m_zoom;
    float viewHeightInTiles = (screenHeight / TILE_SIZE) / m_zoom;

    float left = m_worldX;
    float right = m_worldX + viewWidthInTiles;
    float top = m_worldY;
    float bottom = m_worldY + viewHeightInTiles;

    m_proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

    boundingBox = Rect{left, top, viewWidthInTiles, viewHeightInTiles};
}



void Camera::DispatchEvent(const SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_WHEEL: {
            // (Zoom logic remains the same)
            if (event->wheel.y > 0) { m_zoom *= 2.0f; }
            else if (event->wheel.y < 0) { m_zoom /= 2.0f; }
            m_zoom = std::max(0.125f, std::min(m_zoom, 2.0f));
            updateAfterMove();
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (event->button.button == SDL_BUTTON_MIDDLE) {
                m_isDragging = true;
                SDL_GetMouseState(&m_prevMouseX, &m_prevMouseY);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (event->button.button == SDL_BUTTON_MIDDLE) {
                m_isDragging = false;
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            if (m_isDragging) {
                float currentMouseX, currentMouseY;
                SDL_GetMouseState(&currentMouseX, &currentMouseY);
                
                float deltaX = (currentMouseX - m_prevMouseX) * 4  / TILE_SIZE;
                float deltaY = (currentMouseY - m_prevMouseY) * 4 / TILE_SIZE;

                m_worldX -= deltaX / m_zoom;
                m_worldY -= deltaY / m_zoom;

                m_prevMouseX = currentMouseX;
                m_prevMouseY = currentMouseY;

                updateAfterMove();
            }
            break;
        }
        case SDL_EVENT_KEY_DOWN:

        case SDL_EVENT_WINDOW_RESIZED: {
            screenWidth =  event->window.data1;
            screenHeight = event->window.data2;
        }
    }
}
