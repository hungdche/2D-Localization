#pragma once 

#include <vector>

#include "core/physics.hpp"
#include "core/common.hpp"

struct Ray {
    position _origin;
    angle _rot;
    float _noise;
    bool _is_segment;

    Ray(position o, angle r, bool i) : _origin(o),  _rot(r), _is_segment(i) { }
    ~Ray();

    position getDir() {
        return rayEndpoint(_origin, _rot, 240);
    }

    // reference: The coding train, Coding Challenge #145: 2D Raycasting
    bool isIntersecting(line l, position & intersect){
        // line
        const float x1 = l.s.x; const float y1 = l.s.y;
        const float x2 = l.e.x; const float y2 = l.e.y;

        // ray
        const float x3 = _origin.x; const float y3 = _origin.y;
        const float x4 = getDir().x; const float y4 = getDir().y;

        const float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (denominator == 0) return false;

        const float t = ( (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4) ) / denominator;
        const float u = - ( (x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3) ) / denominator;

        if (!_is_segment) {
            if (t < 0 || t > 1 || u < 0) return false;
            else intersect = {x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
            return true;
        } 
        if (t < 0 || t > 1 || u < 0 || u > 1) return false;
        else intersect = {x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
        return true;
    }
};


