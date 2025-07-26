//
// Created by user on 25.07.2025.
//

#ifndef RECT_H
#define RECT_H
#include <utils/FCoords.h>


struct Rect {
    float x;
    float y;
    float width;
    float height;
    Rect(FCoords p1, FCoords p2) {
        x = p1.x;
        y = p1.y;
        width = p2.x - p1.x;
        height = p2.y - p1.y;
    }
    Rect() {}
    Rect(float x0, float y0, float x1, float y1) : x(x0), y(y0), width(std::abs(x1 - x0)), height(std::abs(y1 - y0)){}

    bool operator==(const Rect& other) const {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    bool IsIn(Coords point) {
        return point.x >= x && point.x <= x + width && point.y >= y && point.y <= y + height;
    }


};



#endif //RECT_H
