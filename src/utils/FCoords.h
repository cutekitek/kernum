//
// Created by user on 14.07.2025.
//

#ifndef COORDS_H
#define COORDS_H
#include <cstddef>

#include "SDL3/SDL_rect.h"
#include <functional>

struct Coords: SDL_Point {
    bool operator==(const Coords &other) const {
        return x == other.x && y == other.y;
    }
};

template <>
struct std::hash<Coords>
{
    std::size_t operator()(const Coords& k) const
    {
        using std::size_t;
        using std::hash;

        return hash<int>()(k.x) ^ hash<int>()(k.y);
    }
};



struct FCoords: SDL_FPoint {
    FCoords() : SDL_FPoint{0, 0} {}
    FCoords(float x, float y): SDL_FPoint{x, y} {}
    FCoords(int x, int y) : SDL_FPoint{(float)x, (float)y} {}

    bool operator==(const FCoords &other) const {
        return x == other.x && y == other.y;
    }
};

template <>
struct std::hash<FCoords>
{
    std::size_t operator()(const FCoords& k) const
    {
        using std::size_t;
        using std::hash;

        return hash<float>()(k.x) ^ hash<float>()(k.y);
    }
};



#endif //COORDS_H
