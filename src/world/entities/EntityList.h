#ifndef ENTITYLIST_H
#define ENTITYLIST_H
#include <memory>
#include <vector>

#include "Entity.h"
#include "EntityIterator.h"


class EntityList {
    std::vector<std::unique_ptr<Entity>> entities;

public:
    void add(std::unique_ptr<Entity> entity) {
        entities.push_back(std::move(entity));
    }

    template <typename T,typename = std::enable_if<std::is_base_of_v<Entity, T>>>
    std::vector<T*> collect() const {
        std::vector<T*> result;
        for (const auto& entity : entities) {
            if (entity->GetType() == EntityTraits<T>::type) {
                result.push_back(static_cast<T*>(entity.get()));
            }
        }
        return result;
    }

    const std::vector<std::unique_ptr<Entity>>& all() const {
        return entities;
    }

    template <typename T>
    EntityRange<T> iter() const {
        return {entities.begin(), entities.end()};
    }

};


#endif //ENTITYLIST_H
