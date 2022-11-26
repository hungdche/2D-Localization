#pragma once 

#include "objects/vehicle.h"

class SimCar : public Object{
private:
    // state
    position _pos;
    angle _rot;
    dimension _dim;

    // visual
    SDL_Texture * _texture;

public:
    SimCar(position init_pos, dimension dim, SDL_Texture * texture);
    ~SimCar();

    // getters
    position & getPos() { return _pos; };
    position getCenter() { return {_pos.x + (_dim.w / 2), _pos.y + (_dim.h / 2)}; }; 
    const angle getAngle() { return _rot; };
    const dimension getDim() { return _dim; };

    SDL_Texture * getTexture() { return _texture; };

    void updateState(state s);
    
};