#include <objects/vehicle.h>

Vehicle::Vehicle(position p, dimension d, SDL_Texture * texture) 
    : _pos{p} , _vel{0}, _rot{0}, _dim{d}, _texture{texture}, _push{0}, _w(0), _accel{0}, last_vel(0){ 
    
    Parser parser("../configs/config.yaml");
    max_speed = parser.max_speed;
    lastFrameTs = SDL_GetTicks(); // in ms
    _imu = std::unique_ptr<IMU>(new IMU(p, parser.noise_a, parser.bias_a, parser.noise_g, parser.bias_g));
    _camera = std::unique_ptr<Camera>(new Camera(getCenter(), _rot, 0.35));
    _raydar = std::unique_ptr<RayDar>(new RayDar(getCenter(), 200));
}

Vehicle::~Vehicle() { 
    SDL_DestroyTexture(_texture);
}

void Vehicle::handleEvent (SDL_Event & e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        switch( e.key.keysym.sym ) {
            case SDLK_UP: is_accel = positive; break;
            case SDLK_DOWN: is_accel = negative; break;
            case SDLK_LEFT: _w -= 1.75; break;
            case SDLK_RIGHT: _w += 1.75; break;
            case SDLK_SPACE: is_accel = deccelerate; break;
        }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ) {
            case SDLK_UP: is_accel = non; break;
            case SDLK_DOWN: is_accel = non; break;
            case SDLK_LEFT: _w += 1.75; break;
            case SDLK_RIGHT: _w -= 1.75; break;
            case SDLK_SPACE: is_accel = non; break;
        }
    }
}

void Vehicle::move (Uint32 current_time) {
    float dt = (current_time - lastFrameTs) / 1000.0f;
    if (is_accel) {
        switch (is_accel) {
            case positive: 
                if (_vel > 0) _push = 75;
                else _push = 125;
                _vel += _push * dt;
                break;
            case negative:
                if (_vel > 0) _push = -125;
                else _push = -75;
                _vel += _push * dt;
                break;
            case deccelerate:
                if (_vel > 0)
                    _vel = std::fmax(_vel - 300 * dt, 0);
                else if (_vel < 0) 
                    _vel = std::fmin(_vel + 300 * dt, 0);
                break;
        }
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
    _rot = radAdd(_rot, _w * dt);
    _accel = (_vel - last_vel)/dt;

    _imu->updateState(_accel, _w, dt);
    _raydar->updateState(getCenter());
    _camera->updateState(getCenter(), _w* dt);
    last_vel = _vel;
}
