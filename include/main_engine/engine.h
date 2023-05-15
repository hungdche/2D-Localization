#pragma once

#include <iostream>
#include <string>
#include <time.h>     
#include <cmath>
#include <vector>

#include "object.h"
#include "obstacle.h"
#include "vehicle.h"
#include "simcar.h"
#include "common.hpp"
#include "physics.hpp"

class Engine {
private:
    // Visual
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    SDL_Texture * _background;
    SDL_Texture * _grid;
    SDL_Surface * _stat_surface;
    SDL_Surface * _speed_surface;
    SDL_Surface * _options_surface;
    TTF_Font * _font;
    bool sim_view = false;
    
    // objects
    std::unique_ptr<Vehicle> car;
    std::unique_ptr<SimCar> scar;
    std::vector<std::unique_ptr<Obstacle>> obstacles; 
    std::vector<line> walls;

    // time
    Uint16 lastUpdate;
    
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
    SimCar * GenerateSimCar();
    
    // time-based events
    SDL_Surface * updateStats();
    SDL_Surface * showSpeed();
    SDL_Surface * showOptions();
    void onResizeCanvas();

    // rendering
    void render(Object * obj, SDL_Texture * texture = NULL);
    void renderCar();
    void renderObstacles();
    void drawRaydars();
    void drawCamRays();
};