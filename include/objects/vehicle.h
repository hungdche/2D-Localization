#pragma once 
#include <SDL.h>
#include <vector>

#include "sensors/sensors.h"
#include "objects/object.h"


class Vehicle : public Object {
private:
    // state
    position _pos; float off_set = 0;
    velocity _vel; float max_speed = 400;
    acceleration _accel; 
    is_accelerate is_accel = non;

    angle _rot; angle _deg_offset;
    dimension _dim;

    // time
    Uint32 lastFrameTs;

    // visuals
    SDL_Texture * _texture;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<RayDar> _raydar;

public:
    Vehicle(position p, dimension d, SDL_Texture * texture);
    ~Vehicle();

    // getters
    position & getPos() { return _pos; };
    position getCenter() { return {_pos.x + (_dim.w / 2), _pos.y + (_dim.h / 2)}; }; 
    const float getOffset() { return off_set;};
    const angle getAngle() { return _rot; };
    const dimension getDim() { return _dim; };

    SDL_Texture * getTexture() { return _texture; };
    std::shared_ptr<Camera> getCamera() {return _camera; };
    std::shared_ptr<RayDar> getRayDar() {return _raydar; };

    // setters
    void setLastUpdate (Uint32 l) { lastFrameTs = l; };
    void setMaxSpeed (float v) { max_speed = v; };
    
    // event actions
    void handleEvent(SDL_Event & e);
    void move();
    void bump();
};
