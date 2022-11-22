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
    // variables
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    SDL_Texture * _background;
    SDL_Texture * _grid;
    Vehicle * car;
    std::vector<Obstacle *> obstacles; 
    std::vector<line> walls;
    
    // helper functions
    SDL_Texture * createTexture(dimension d, const SDL_Color * color, const SDL_Color * borderColor = NULL);
    SDL_Texture * drawGrid (dimension d, const SDL_Color * background, const SDL_Color * grid, int size);
    void GenerateObstacle();
    Vehicle * GenerateCar(dimension d);
    
    void render(Object * obj, SDL_Texture * texture = NULL);
    void renderCar();

public:
    Engine(const char * name);
    ~Engine();

    void run();
    void handleEvent(SDL_Event & e);
};