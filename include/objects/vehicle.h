#pragma once 
#include <SDL.h>
#include <vector>

#include "sensors/ray.hpp"
#include "objects/object.h"


class Vehicle : public Object {
private:
    position _pos; float _forward_offset;
    angle _rot; angle _deg_offset;
    dimension _dim;

    SDL_Texture * _texture;
    std::vector<Ray *> rays;

public:
    Vehicle(position p, dimension d, SDL_Texture * texture);
    ~Vehicle();

    position & getPos() { return _pos; };
    position getCenter() { return {_pos.x + (_dim.w / 2), _pos.y + (_dim.h / 2)}; }; 
    const float getOffset() { return _forward_offset;};
    const angle getAngle() { return _rot; };
    const dimension getDim() { return _dim; };
    
    SDL_Texture * getTexture() { return _texture; };
    std::vector<Ray *> getRays() & {return rays; };
    void handleEvent(SDL_Event & e);
    void move();
    void bump();
};
