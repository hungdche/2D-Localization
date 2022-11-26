#include "objects/simcar.h"

SimCar::SimCar(position init_pos, dimension dim, SDL_Texture * texture) 
    : _pos{init_pos}, _rot{0}, _dim{dim}, _texture{texture} { }

SimCar::~SimCar() {
    SDL_DestroyTexture(_texture);
}

void SimCar::updateState(state s) {
    srand(time(NULL));

    _pos.x = s._pos.x + rand() % 300;
    _pos.y = s._pos.y + rand() % 300;
    _rot = s._yaw + rand() % 30;
}