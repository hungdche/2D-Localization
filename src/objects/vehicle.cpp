#include <objects/vehicle.h>

Vehicle::Vehicle(position p, dimension d, SDL_Texture * texture) 
    : _pos{p} , _rot{0}, _dim{d}, _texture{texture}, _accel{0}, _deg_offset(0) { 

    lastFrameTs = SDL_GetTicks(); // in ms
    _camera = std::unique_ptr<Camera>(new Camera(getCenter(), _rot, 20));
    _raydar = std::unique_ptr<RayDar>(new RayDar(getCenter()));
}

Vehicle::~Vehicle() { 
    SDL_DestroyTexture(_texture);
}

void Vehicle::handleEvent (SDL_Event & e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        switch( e.key.keysym.sym ) {
            case SDLK_UP: is_accel = positive; break;
            case SDLK_DOWN: is_accel = negative; break;
            case SDLK_LEFT: _deg_offset -= 100; break;
            case SDLK_RIGHT: _deg_offset += 100; break;
            case SDLK_SPACE: is_accel = deccelerate; break;
        }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ) {
            case SDLK_UP: is_accel = non; break;
            case SDLK_DOWN: is_accel = non; break;
            case SDLK_LEFT: _deg_offset += 100; break;
            case SDLK_RIGHT: _deg_offset -= 100; break;
            case SDLK_SPACE: is_accel = non; break;
        }
    }
}

void Vehicle::move (Uint32 current_time) {
    float dt = (current_time - lastFrameTs) / 1000.0f;
    if (is_accel) {
        switch (is_accel) {
            case positive: 
                if (_vel > 0) _accel = 75;
                else _accel = 125;
                break;
            case negative:
                if (_vel > 0) _accel = -125;
                else _accel = -75;
                break;
            case deccelerate:
                if (_vel > 0) _accel = -300;
                else _accel = 300;
                break;
        }
        _vel += _accel * dt;
        _vel = std::fmin (_vel, max_speed);
        _vel = std::fmax (_vel, -max_speed);
    }

    off_set = _vel * dt;
    float velx = _vel * getX(getAngle()); 
    float vely = _vel * getY(getAngle());
    // move in x direction
    if (_pos.x+velx * dt >= 0 && _pos.x +velx * dt+ _dim.w < ScreenDim.w && _pos.y + vely * dt >= 0 && _pos.y+vely * dt + _dim.h < ScreenDim.h) {
        _pos.x += velx * dt;
        _pos.y += vely * dt;
    } else {
        _vel = -_vel/10;
    }

    // move in y direction
    // _pos.y += vely * dt;
        

    _rot =  std::fmod((_rot + _deg_offset * dt) + 180, 360);
    if (_rot < 0) _rot += 360;
    _rot -= 180;
    _camera->updateState(getCenter(), _deg_offset* dt);
    _raydar->updateState(getCenter());
}
