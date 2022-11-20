#pragma once
#include <SDL.h>
#include <iostream>

#include "core/math.hpp"

class Object {
public:
    virtual position getPos() { return {0,0}; };
    virtual dimension getDim() { return {10, 10}; };
    virtual SDL_Texture * getTexture() { return NULL; };
    virtual void handleEvent(SDL_Event & e) = 0;
    virtual ~Object() {}
};