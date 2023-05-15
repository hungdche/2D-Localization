#pragma once 

#include <stdlib.h>
#include <cmath>

#include "common.hpp"

inline float distance(position a, position b) {
    return std::sqrt(std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2));
}

inline float angleBtw(position start, position end) {
    float x_offset = end.x - start.x;
    float y_offset = end.y - start.y;
    return std::atan2(x_offset,- y_offset);
}

inline position rayEndpoint(position a, angle theta, float distance) {
    // theta = theta * M_PI / 180.0;
    float x_offset = distance * std::sin(theta);
    float y_offset = -distance * std::cos(theta);
    position endpt = {a.x + x_offset, a.y + y_offset};
    return endpt;
}

inline int isInDir (position start, position end, angle theta) {
    float dist = distance(start, end);
    // float temp_angle = angleBtw(start, end) * (180 / M_PI);
    float temp_angle = angleBtw(start, end);

    if ((temp_angle > 0 && theta > 0) || (temp_angle <= 0 && theta <= 0)) return 1;
    return -1;
}

inline float getX (angle a) {
    return std::sin(a);
}

inline float getY (angle a) {
    return -std::cos(a);
}

inline float toDeg (angle a) {
    return a * 180.0 / M_PI;
}

inline float radAdd (angle _rot, float b) {
    _rot =  std::fmod((_rot + b) + M_PI, 2 * M_PI);
    if (_rot < 0) _rot += 2 * M_PI;
    _rot -= M_PI;
    return _rot;
}