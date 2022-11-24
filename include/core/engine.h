#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <time.h>     
#include <cmath>
#include <vector>

#include "objects/object.h"
#include "objects/obstacle.h"
#include "objects/vehicle.h"
#include "core/common.hpp"
#include "core/physics.hpp"

class Engine {
private:
    // SDL 
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    SDL_Texture * _background;
    SDL_Texture * _grid;
    
    // objects
    Vehicle * car;
    std::vector<Obstacle *> obstacles; 
    std::vector<line> walls;

public:
    Engine(const char * name);
    ~Engine();

    void run();
    void handleEvent(SDL_Event & e);

private:
    // init events
    SDL_Texture * createTexture(dimension d, const SDL_Color * color, const SDL_Color * borderColor = NULL);
    SDL_Texture * drawGrid (dimension d, const SDL_Color * background, const SDL_Color * grid, int size);
    void GenerateObstacle();
    Vehicle * GenerateCar(dimension d);
    
    // time-based events
    void render(Object * obj, SDL_Texture * texture = NULL);
    void renderCar();
    void onResizeCanvas();
};