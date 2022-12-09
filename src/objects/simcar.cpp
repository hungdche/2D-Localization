#include "objects/simcar.h"

SimCar::SimCar(position init_pos, angle r, dimension dim, SDL_Texture * texture) 
    : _pos{init_pos}, _rot{r}, _dim{dim}, _texture{texture}, lastTime(SDL_GetTicks()) { }

SimCar::~SimCar() {
    SDL_DestroyTexture(_texture);
}

void SimCar::feedIMU(control s) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::normal_distribution<float> dist(0.0, 0.17); 

    float dt = (s.timestamp - lastTime) / 1000.0f;
    
    _pos.x += s._vel * getX(_rot) * dt + dist(gen);
    _pos.y += s._vel * getY(_rot) * dt + dist(gen);
    _rot += s._yaw * dt + dist(gen);

    lastTime = s.timestamp;
}

void SimCar::feedCamera(std::vector<position> data) {
    
}

void SimCar::feedRaydar(std::vector<PtdCld> data) {

}

