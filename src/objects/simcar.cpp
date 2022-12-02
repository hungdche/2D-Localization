#include "objects/simcar.h"

SimCar::SimCar(position init_pos, dimension dim, SDL_Texture * texture) 
    : _pos{init_pos}, _rot{0}, _dim{dim}, _texture{texture}, lastTime(0) {
        std::cout << "Hi";
     }

SimCar::~SimCar() {
    SDL_DestroyTexture(_texture);
}

void SimCar::updateState(state s) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::normal_distribution<float> dist(0.0, 0.03); 

    float dt = (s.timestamp - lastTime) / 1000.0f;
    
    _pos.x += s._vel * getX(_rot) * dt;
    _pos.y += s._vel * getY(_rot) * dt;
    _rot += s._yaw * dt + dist(gen);

    lastTime = s.timestamp;
}


