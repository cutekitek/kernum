//
// Created by user on 13.07.2025.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "Texture.h"
#include "SDL3/SDL_render.h"


class TextureManager {
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    std::filesystem::path basePath;
    SDL_Renderer* renderer;
    public:
    TextureManager(const std::filesystem::path &base_path, SDL_Renderer *renderer)
        : basePath(base_path),
          renderer(renderer) {
    }

    ~TextureManager() = default;
    std::shared_ptr<Texture> LoadTexture(const std::string &fileName);
};



#endif //TEXTUREMANAGER_H
