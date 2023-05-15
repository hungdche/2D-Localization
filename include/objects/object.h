#pragma once
#include <iostream>

#include "common.hpp"
#include "physics.hpp"
#include "types.hpp"

class Object {
public:
    virtual position & getPos() = 0;
    virtual const dimension getDim() = 0;
    virtual const angle getAngle() {return 0; };
    virtual SDL_Texture * getTexture() { return NULL; };
    virtual ~Object() {}
};