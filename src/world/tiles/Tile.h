

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

    [[nodiscard]] const std::vector<const Texture*>& get_texture_variants() const {
        return variants;
    }

    bool is_solid() const {
        return solid;
    }

    std::string get_name() const {
        return name;
    }

    Tile(const uint32_t tileId, const std::string& name, const std::vector<const Texture*>& variants, const bool solid) : tile_id(tileId), variants(variants), name(name), solid(solid) {}


private:
    uint32_t tile_id;
    std::vector<const Texture*> variants;
    const std::string& name;
    bool solid;

};



#endif //TILE_H
