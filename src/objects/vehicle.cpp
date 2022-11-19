#include <objects/vehicle.h>

Vehicle::Vehicle(position p, dimension d, SDL_Texture * texture) 
    : _pos(p) , _rot({0}), _dim(d), _texture(texture) { }

Vehicle::~Vehicle() { 
    SDL_DestroyTexture(_texture);
}

