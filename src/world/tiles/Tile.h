

#ifndef TILE_H
#define TILE_H
#include <cstdint>
#include <string>
#include <memory>

#include "../../graphics/Texture.h"


class Tile {
public:
    uint32_t get_tile_id() const {
        return tile_id;
    }

    [[nodiscard]] const std::shared_ptr<Texture> get_texture() const {
        return texture;
    }

    bool is_solid() const {
        return solid;
    }

    std::string get_name() const {
        return name;
    }

    Tile(const uint32_t tileId, const std::string& name, const std::shared_ptr<Texture>& texture, const bool solid) : tile_id(tileId), texture(texture), name(name), solid(solid) {

    }


private:
    uint32_t tile_id;
    std::shared_ptr<Texture> texture;
    const std::string& name;
    bool solid;

};



#endif //TILE_H
