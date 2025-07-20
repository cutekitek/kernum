#include "WorldRenderer.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <iostream>
#define TILE_SIZE 32

void WorldRenderer::render(SDL_Renderer *renderer, SDL_Window *window) {
    int tileSize = TILE_SIZE / 2;
    int screen_w, screen_h;
    SDL_GetWindowSize(window, &screen_w, &screen_h);
    int startTileX = (int) (cameraX / tileSize);
    int startTileY = (int) (cameraY / tileSize);

    int endTileX = (int) ((cameraX + screen_w) / tileSize) + 1;
    int endTileY = (int) ((cameraY + screen_h) / tileSize) + 1;

    auto chunkSize = chunkManager->GetChunkSize();


    SDL_FRect dstrect;

    for (int tileX = startTileX; tileX < endTileX; tileX+=chunkSize) {
        for (int tileY = startTileY; tileY < endTileY; tileY+=chunkSize) {
            auto tiles = chunkManager->LoadChunk(FCoords{tileX, tileY})->GetTiles();
            for (int i = 0; i < chunkSize; i++) {
                for (int j = 0; j < chunkSize; j++) {
                    dstrect.x = (float) ((tileX + i) * tileSize) - cameraX;
                    dstrect.y = (float) ((tileY + j) * tileSize) - cameraY;
                    dstrect.w = tileSize;
                    dstrect.h = tileSize;
                    auto text = tileManager->GetTileById(tiles[j * chunkSize + i]).get_texture().get_texture();
                    if (!SDL_RenderTexture(renderer, text, nullptr, &dstrect)) {
                        std::cout << "Render error:" << SDL_GetError() << std::endl;
                        throw SDL_GetError();
                    };
                }
            }
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
