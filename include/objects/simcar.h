#pragma once 

#include "objects/vehicle.h"
#include "core/algorithm.h"
#include "core/parser.hpp"
#include <random>

class SimCar : public Object{
private:
    // state
    state estimated;
    state predicted;
    state measured;

    dimension _dim;
    velocity _vel;
    velocity _w;

    // state estimation
    std::unique_ptr<EKF> alg;

    // visual
    SDL_Texture * _texture;

public:
    SimCar(state s, velocity vel, velocity w, dimension dim, SDL_Texture * texture);
    ~SimCar();

    // getters
    position & getPos() { return estimated._pos; };
    position getCenter() { return {estimated._pos.x + (_dim.w / 2), estimated._pos.y + (_dim.h / 2)}; }; 
    const angle getAngle() { return estimated._rot; };
    const dimension getDim() { return _dim; };

    state getPredState() { return predicted; };
    state getMeasState() { return measured; };

    SDL_Texture * getTexture() { return _texture; };

    void feedIMU(control c, state s);
    
};