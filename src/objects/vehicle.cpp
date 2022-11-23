#include <objects/vehicle.h>

Vehicle::Vehicle(position p, dimension d, SDL_Texture * texture) 
    : _pos{p} , _rot{0}, _dim{d}, _texture{texture}, _forward_offset(0), _deg_offset(0) { 

    for (float i = -20; i <= 20; i+=1) {
        position center = getCenter();
        rays.push_back(new Ray(center, i));
    }
}

Vehicle::~Vehicle() { 
    SDL_DestroyTexture(_texture);
}

void Vehicle::handleEvent (SDL_Event & e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        switch( e.key.keysym.sym ) {
            case SDLK_UP: _forward_offset += 0.7; break;
            case SDLK_DOWN: _forward_offset -= 0.7; break;
            case SDLK_LEFT: _deg_offset -= 1; break;
            case SDLK_RIGHT: _deg_offset += 1; break;
        }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ) {
            case SDLK_UP: _forward_offset -= 0.7; break;
            case SDLK_DOWN: _forward_offset += 0.7; break;
            case SDLK_LEFT: _deg_offset += 1; break;
            case SDLK_RIGHT: _deg_offset -= 1; break;
        }
    }
}

void Vehicle::move () {
    _pos.x += _forward_offset * std::sin(getAngle() * M_PI / 180.0);
    if (_pos.x < 0 || _pos.x + _dim.w > ScreenDim.w)
        _pos.x -= _forward_offset * std::sin(getAngle() * M_PI / 180.0);

    _pos.y += -_forward_offset * std::cos(getAngle() * M_PI / 180.0);
    if (_pos.y < 0 || _pos.y + _dim.h > ScreenDim.h) 
        _pos.y -= -_forward_offset * std::cos(getAngle() * M_PI / 180.0);

    _rot =  std::fmod((_rot + _deg_offset), 360);

    for (auto & ray : rays) {
        ray->_origin = getCenter();
        ray->_rot = std::fmod((ray->_rot + _deg_offset), 360);
    }
}


