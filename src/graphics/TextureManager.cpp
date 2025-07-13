//
// Created by user on 13.07.2025.
//

#include "TextureManager.h"
#include <SDL3_image/SDL_image.h>


std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string &fileName) {
    auto path = (basePath / fileName).string();
    auto texture = textures[path];
    if (texture != nullptr) {
        return texture;
    }
    SDL_Texture* text = IMG_LoadTexture(renderer, path.c_str());
    if (text == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture file '%s'! SDL_image Error: %s", path.c_str(), SDL_GetError());
    }
    texture = std::make_shared<Texture>(text);
    textures[path] = texture;
    return texture;
}
