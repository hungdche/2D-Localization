#pragma once 

#include "objects/object.h"

class Obstacle : public Object {
private:
    // state
    position _pos; 
    dimension _dim;

    // mouse movement
    bool isDragged = false; 
    position _prev_mouse;

    // visual 
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