#pragma once 
#include <SDL.h>

#include "objects/object.h"

class Vehicle : public Object {
private:
    position _pos;
    velocity _vel;
    angle _rot;
    angle _deg_offset;
    dimension _dim;
    SDL_Texture * _texture;

public:
    Vehicle(position p, dimension d, SDL_Texture * texture);
    ~Vehicle();

    position & getPos() { return _pos; };
    const angle getAngle() { return _rot; };
    velocity & getVel() { return _vel; };
    const dimension getDim() { return _dim; };
    SDL_Texture * getTexture() { return _texture; };

    void handleEvent(SDL_Event & e);
    void move();
    void bump();
};
