#include <objects/vehicle.h>

Vehicle::Vehicle(position p, dimension d, SDL_Texture * texture) 
    : _pos(p) , _rot({0}), _dim(d), _vel({0,0}), _texture(texture) { }

Vehicle::~Vehicle() { 
    SDL_DestroyTexture(_texture);
}

void Vehicle::handleEvent (SDL_Event & e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        switch( e.key.keysym.sym ) {
            case SDLK_UP: _vel.y -= 0.7; break;
            case SDLK_DOWN: _vel.y += 0.7; break;
            case SDLK_LEFT: _vel.x -= 0.7; break;
            case SDLK_RIGHT: _vel.x += 0.7; break;
            case SDLK_a: _deg_offset -= 1; break;
            case SDLK_d: _deg_offset += 1; break;
        }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ) {
            case SDLK_UP: _vel.y += 0.7; break;
            case SDLK_DOWN: _vel.y -= 0.7; break;
            case SDLK_LEFT: _vel.x += 0.7; break;
            case SDLK_RIGHT: _vel.x -= 0.7; break;
            case SDLK_a: _deg_offset += 1; break;
            case SDLK_d: _deg_offset -= 1; break;
        }
    }
}

void Vehicle::move () {
    _pos.x += _vel.x;
    if (_pos.x < 0 || _pos.x + _dim.w > ScreenDim.w)
        _pos.x -= _vel.x;

    _pos.y += _vel.y;
    if (_pos.y < 0 || _pos.y + _dim.h > ScreenDim.h) 
        _pos.y -= _vel.y;

    _rot += _deg_offset;
}


