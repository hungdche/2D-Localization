#pragma once 
#include <SDL.h>

#include "objects/object.h"

class Vehicle : public Object {
private:
    position _pos;
    velocity _vel;
    acceleration _accel = {0,0};
    rotation _rot;
    dimension _dim;
    SDL_Texture * _texture;

public:
    Vehicle(position p, dimension d, SDL_Texture * texture);
    ~Vehicle();

    position getPos() { return _pos; };
    rotation getRot() { return {0}; };
    dimension getDim() { return _dim; };
    SDL_Texture * getTexture() { return _texture; };

    void handleEvent(SDL_Event & e);
    void move();
};
