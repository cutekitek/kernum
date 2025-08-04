#ifndef ENTITY_H
#define ENTITY_H
#include "graphics/Texture.h"
#include "utils/FCoords.h"

enum class EntityType {
    None = 0,
  Ore = 1,
};

template <typename T>
struct EntityTraits;

class Entity {
public:
    virtual ~Entity() = default;
    virtual EntityType GetType() {
        return EntityType::None;
    };
    virtual const Texture* GetTexture() {
        return nullptr;
    };
    virtual const FCoords& GetPosition() {
        return FCoords{};
    };
    virtual void Update(uint64_t tick) {};
};


#endif //ENTITY_H
