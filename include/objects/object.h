#pragma once
#include <SDL.h>
#include <iostream>

#include "core/common.hpp"
#include "core/physics.hpp"

class Object {
public:
    virtual position & getPos() = 0;
    virtual const dimension getDim() = 0;
    virtual const angle getAngle() {return 0; };
    virtual SDL_Texture * getTexture() { return NULL; };
    virtual ~Object() {}
};