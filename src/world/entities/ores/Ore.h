#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics/Texture.h"
#include "world/entities/Entity.h"


class Ore:public Entity {
    uint32_t remaining;
    FCoords position;
    const Texture* texture;

    public:
    const Texture* GetTexture() override {
        return texture;
    };
    Ore(const Texture* texture, const uint32_t remaining, const FCoords pos):remaining(remaining), position(pos), texture(texture) {};
    uint32_t GetRemaining() const {return remaining;};
    const FCoords& GetPosition() override {return position;}
    EntityType GetType() override {
        return EntityType::Ore;
    }
    void Update(uint64_t tick) override{}
};

template <>
struct EntityTraits<Ore> {
    static constexpr EntityType type = EntityType::Ore;
};


#endif
