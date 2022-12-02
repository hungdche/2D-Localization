#pragma once 

#include <stdlib.h>
#include <cmath>

#include "core/common.hpp"

inline float distance(position a, position b) {
    return std::sqrt(std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2));
}

inline float angleBtw(position start, position end) {
    float x_offset = end.x - start.x;
    float y_offset = end.y - start.y;
    return std::atan2(x_offset,- y_offset);
}

inline position rayEndpoint(position a, angle theta, float distance) {
    theta = theta * M_PI / 180.0;
    float x_offset = distance * std::sin(theta);
    float y_offset = -distance * std::cos(theta);
    position endpt = {a.x + x_offset, a.y + y_offset};
    return endpt;
}

inline int isInDir (position start, position end, angle theta) {
    float dist = distance(start, end);
    float temp_angle = angleBtw(start, end) * (180 / M_PI);

    if ((temp_angle > 0 && theta > 0) || (temp_angle <= 0 && theta <= 0)) return 1;
    return -1;
}

inline float getX (angle a) {
    return std::sin(a * M_PI / 180.0);
}

inline float getY (angle a) {
    return -std::cos(a * M_PI / 180.0);
}
