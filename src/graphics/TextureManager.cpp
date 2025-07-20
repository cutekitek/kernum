#include "TextureManager.h"
#include <SDL3/SDL_error.h>
#include <SDL3_image/SDL_image.h>
#include <memory>


const Texture* TextureManager::LoadTexture(const std::string &fileName) {
    auto path = (basePath / fileName).string();
    auto text = textures.find(fileName);
    if (text != textures.end()) {
        return text->second.get();
    }
    SDL_Texture* sdlText = IMG_LoadTexture(renderer, path.c_str());
    if (sdlText == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture file '%s'! SDL_image Error: %s", path.c_str(), SDL_GetError());
    }
    auto texture = std::make_unique<Texture>(sdlText);
    textures[path] = std::move(texture);
    return textures[path].get();
}
