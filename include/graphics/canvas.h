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

class Canvas {
private:
    // variables
    dimension _dim;
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    SDL_Texture * _background;
    SDL_Texture * _grid;
    std::vector<Obstacle *> obstacles; 
    
    // helper functions

public:
    Canvas(const char * name, dimension d);
    ~Canvas();

    void clear();
    void present();
    void render(Object & obj, SDL_Texture * texture = NULL);
    SDL_Texture * createTexture(dimension d, const SDL_Color * color, const SDL_Color * borderColor = NULL);
    SDL_Texture * drawGrid (dimension d, const SDL_Color * background, const SDL_Color * grid, int size);
    void handleEvent(SDL_Event & e);
    void GenerateObstacle();
    Vehicle * GenerateCar(dimension d);
};