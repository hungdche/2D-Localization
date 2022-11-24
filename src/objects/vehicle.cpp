#include <objects/vehicle.h>

Vehicle::Vehicle(position p, dimension d, SDL_Texture * texture) 
    : _pos{p} , _rot{0}, _dim{d}, _texture{texture}, _accel{0}, _deg_offset(0) { 

    lastFrameTs = SDL_GetTicks(); // in ms
    _camera = std::shared_ptr<Camera>(new Camera(getCenter(), _rot, 40));
    _raydar = std::shared_ptr<RayDar>(new RayDar(getCenter()));
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

void Vehicle::move () {
    float dt = (SDL_GetTicks() - lastFrameTs) / 1000.0f;
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
    
    printf("%f %f\n", _vel, _accel);
    off_set = _vel * dt;
    float velx = _vel * getX(getAngle()); 
    float vely = _vel * getY(getAngle());
    // move in x direction
    _pos.x += velx * dt;
    if (_pos.x < 0 || _pos.x + _dim.w > ScreenDim.w)
        _pos.x -= velx * dt;

    // move in y direction
    _pos.y += vely * dt;
    if (_pos.y < 0 || _pos.y + _dim.h > ScreenDim.h)
        _pos.y -= vely * dt;

    _rot =  std::fmod((_rot + _deg_offset * dt), 360);
    _camera->updateState(getCenter(), _deg_offset* dt);
    _raydar->updateState(getCenter());
}


