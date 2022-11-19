#pragma once
#include <SDL.h>
#include <iostream>

#include "core/math.hpp"

class Object {
public:
    virtual position getPosition() { return {0,0}; };
    virtual rotation getRotation() { return {0}; }
    virtual dimension getDimension() { return {10, 10}; };
    virtual SDL_Texture * getTexture() { return NULL; };
    void printAttribute() {
        printf("POS: %d %d ROT: %f DIM: %d %d\n", getPosition().x, getPosition().y, getRotation().angle, getDimension().w, getDimension().h);
    };
    virtual ~Object() {}
};