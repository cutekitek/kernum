#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H
#include <memory>

#include "SDL3/SDL_render.h"
#include "../world/chunks/ChunkManager.h"

class WorldRenderer {
    std::shared_ptr<ChunkManager> chunkManager;
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float scale = 2.0f;
    public:
    WorldRenderer(const std::shared_ptr<ChunkManager> &chunkManager): chunkManager(chunkManager) {};
    ~WorldRenderer() = default;
    void render(SDL_Renderer *renderer, SDL_Window *window);

    void dispatchEvent(SDL_Event *event);
};



#endif //WORLDRENDERER_H
