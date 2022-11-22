#pragma once 

#include <SDL.h>

#include "objects/object.h"

class Obstacle : public Object {
private:
    position _pos; dimension _dim;
    bool isDragged = false; position _prev_mouse;

    SDL_Texture * _texture;
    

public:
    Obstacle(position p, dimension d, SDL_Texture * texture);
    ~Obstacle();

    position & getPos() { return _pos; };
    const dimension getDim() { return _dim; };
    SDL_Texture * getTexture() { return _texture; };
    std::vector<line> getWalls();

    void handleEvent(SDL_Event & e);
    void drag();
};