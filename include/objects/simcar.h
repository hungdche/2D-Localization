#pragma once 

#include "objects/vehicle.h"
#include <random>

class SimCar : public Object{
private:
    // state
    position _pos;
    angle _rot;
    dimension _dim;
    velocity _vel;
    velocity _w;

    // visual
    SDL_Texture * _texture;
    Uint32 lastTime;

public:
    SimCar(position init_pos, angle r, dimension dim, SDL_Texture * texture);
    ~SimCar();

    // getters
    position & getPos() { return _pos; };
    position getCenter() { return {_pos.x + (_dim.w / 2), _pos.y + (_dim.h / 2)}; }; 
    const velocity getVel() { return _vel; };
    const velocity getW() { return _w; }; 
    const angle getAngle() { return _rot; };
    const dimension getDim() { return _dim; };

    SDL_Texture * getTexture() { return _texture; };

    void feedIMU(control s);
    void feedCamera(std::vector<position> data);
    void feedRaydar(std::vector<PtdCld> data);
    
};