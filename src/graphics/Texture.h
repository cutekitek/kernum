#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

#include "SDL3/SDL_render.h"


class Texture {
    SDL_Texture* texture;

public:
    [[nodiscard]] SDL_Texture * get_texture() const {
        return texture;
    }

    Texture(SDL_Texture *texture):
          texture(texture) {
    }

    ~Texture() {
        SDL_DestroyTexture(texture);
    };


};



#endif //TEXTURE_H
