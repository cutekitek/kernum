#include "WorldRenderer.h"
#define TILE_SIZE 32

void WorldRenderer::render(SDL_Renderer *renderer, SDL_Window *window) {
    int tileSize = TILE_SIZE / scale;
    int screen_w, screen_h;
    SDL_GetWindowSize(window, &screen_w, &screen_h);
    int startTileX = (int) (cameraX / tileSize);
    int startTileY = (int) (cameraY / tileSize);

    int endTileX = (int) ((cameraX + screen_w) / tileSize) + 1;
    int endTileY = (int) ((cameraY + screen_h) / tileSize) + 1;

    SDL_FRect dstrect;

    for (int tileX = startTileX; tileX < endTileX; tileX++) {
        for (int tileY = startTileY; tileY < endTileY; tileY++) {
            auto tile = this->chunkManager->GetTileByCoords(FCoords{tileX, tileY});

            if (tile == nullptr) continue;

            dstrect.x = (float) (tileX * tileSize) - cameraX;
            dstrect.y = (float) (tileY * tileSize) - cameraY;
            dstrect.w = tileSize;
            dstrect.h = tileSize;

            SDL_RenderTexture(renderer, tile->get_texture()->get_texture(), nullptr, &dstrect);
        }
    }
}

void WorldRenderer::dispatchEvent(SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.key) {
                case SDLK_LEFT:
                    cameraX -= 1;
                case SDLK_RIGHT:
                    cameraX += 1;
                case SDLK_UP:
                    cameraY -= 1;
                case SDLK_DOWN:
                    cameraY += 1;
            }
        case SDL_EVENT_MOUSE_WHEEL:
            if (event-> wheel.integer_y > 0) {
                scale = std::min(scale * 2, (float)32);
            } else {
                scale = std::max(scale / 2, (float)1);
            }
    }
}
