#pragma once 
#include <SDL.h>

#include "objects/object.h"

class Vehicle : public Object {
private:
    position _pos;
    rotation _rot;
    dimension _dim;
    SDL_Texture * _texture;

public:
    Vehicle(position p, dimension d, SDL_Texture * texture);
    ~Vehicle();

    position getPosition() { return _pos; };
    rotation getRotation() { return {0}; };
    dimension getDimension() { return _dim; };
    SDL_Texture * getTexture() { return _texture; };
};
