#pragma once 

#include <stdlib.h>
#include <cmath>

#include "core/common.hpp"

inline float distance(position a, position b) {
    return std::sqrt(std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2));
}

inline position line_intersect (position a1, position a2, position b1, position b2) {
    float den = (a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x);
    if (den == 0) return {-1,-1};
    float num_y = (a1.x*a2.y - a1.y*a2.x)*(b1.y-b2.y) - (b1.x*b2.y - b1.y*b2.x)*(a1.y-a2.y);
    float num_x = (a1.x*a2.y - a1.y*a2.x)*(b1.x-b2.x) - (b1.x*b2.y - b1.y*b2.x)*(a1.x-a2.x);

    position intersect = {num_x / den, num_y / den};
    if (intersect.x > ScreenDim.w || intersect.x < 0) intersect.x = -1;
    if (intersect.y > ScreenDim.h || intersect.y < 0) intersect.y = -1;
    return intersect;
}

inline position rayEndpoint(position a, angle theta, float distance) {
    theta = theta * M_PI / 180.0;
    float x_offset = distance * std::sin(theta);
    float y_offset = -distance * std::cos(theta);
    position endpt = {a.x + x_offset, a.y + y_offset};
    return endpt;
}

// midpoint algorithms
inline void drawCircle (int radius, int x0, int y0, std::vector<SDL_Point> & ret) {
    ret.clear();
    int diameter = radius * 2;

    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - diameter;

    SDL_Point temp;
    while (x >= y) {
        temp = {x0 + x, y0 + y}; ret.push_back(temp);
        temp = {x0 + y, y0 + x}; ret.push_back(temp);
        temp = {x0 - y, y0 + x}; ret.push_back(temp);
        temp = {x0 - x, y0 + y}; ret.push_back(temp);
        temp = {x0 - x, y0 - y}; ret.push_back(temp);
        temp = {x0 - y, y0 - x}; ret.push_back(temp);
        temp = {x0 + y, y0 - x}; ret.push_back(temp);
        temp = {x0 + x, y0 + y}; ret.push_back(temp);

        if (err <= 0) {
            y++;
            err += y;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - diameter;
        }
    }
}

inline position xBeforeEndpoint(position a, angle theta, float distance) {
    theta = theta * M_PI / 180.0;
    float x_offset = distance * std::sin(theta);
    float y_offset = -distance * std::cos(theta);
    position endpt = {a.x - x_offset, a.y - y_offset};
    return endpt;
}

inline float getX (angle a) {
    return std::sin(a * M_PI / 180.0);
}

inline float getY (angle a) {
    return -std::cos(a * M_PI / 180.0);
}
