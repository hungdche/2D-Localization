#include <iostream>
#include <string>
#include <SDL.h>
#include <cmath>

#include "graphics/color.hpp"
#include "graphics/canvas.h"
#include "core/noise.hpp"

Canvas::Canvas(const char * name, dimension d) : _dim(d){
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_EVENTS)) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
    _window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, d.w, d.h, 0);
    _renderer = SDL_CreateRenderer(_window, -1,  SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    _background = drawGrid(d, &SOFT_BLACK, &DARK_GREY, 40);
}

Canvas::~Canvas() {
    SDL_DestroyTexture(_background);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

SDL_Texture * Canvas::drawGrid (dimension d, const SDL_Color * bc, const SDL_Color * gc, int size) {
    SDL_Texture * grid = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, d.w, d.h);
    SDL_SetRenderTarget(_renderer, grid);

    // background
    SDL_SetRenderDrawColor(_renderer, bc->r, bc->g, bc->b, bc->a);  
    SDL_RenderClear(_renderer);

    // grid
    SDL_SetRenderDrawColor(_renderer, gc->r, gc->g, gc->b, gc->a); 
    for (int i = 0; i < d.w; i+=size) SDL_RenderDrawLine(_renderer, i, 0, i, d.h); // verticle
    for (int i = 0; i < d.h; i+=size) SDL_RenderDrawLine(_renderer, 0, i, d.w, i); // horizontal
    
    // border
    SDL_Rect border = {0, 0, d.w, d.h};
    SDL_RenderDrawRect(_renderer, &border);

    SDL_SetRenderTarget(_renderer, NULL);
    return grid;
}

void Canvas::clear() {
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _background, NULL, NULL);
    for (auto ob : obstacles) render(*ob);
}

void Canvas::present() {
    SDL_RenderPresent(_renderer);
}

void Canvas::render(Object & obj, SDL_Texture * texture) {
    SDL_Rect src = {0, 0, obj.getDim().w, obj.getDim().h};
    SDL_FRect dst = {obj.getPos().x, obj.getPos().y, (float)obj.getDim().w, (float)obj.getDim().h};
    if (texture) SDL_SetRenderTarget(_renderer, texture);
    SDL_RenderCopyF(_renderer, obj.getTexture(), &src, &dst);
    if (texture) SDL_SetRenderTarget(_renderer, NULL);
}

SDL_Texture * Canvas::createTexture(dimension d, const SDL_Color * color, const SDL_Color * borderColor) {
    SDL_Texture * texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, d.w, d.h);
    SDL_SetRenderTarget(_renderer, texture);

    if (borderColor) {
        SDL_SetRenderDrawColor(_renderer, borderColor->r, borderColor->g, borderColor->b, borderColor->a);  
        SDL_RenderClear(_renderer);

        SDL_SetRenderDrawColor(_renderer, color->r, color->g, color->b, color->a); 
        SDL_Rect border = {2, 2, d.w-4, d.h-4};
        SDL_RenderFillRect(_renderer, &border);
    } else {
        SDL_SetRenderDrawColor(_renderer, color->r, color->g, color->b, color->a);  
        SDL_RenderClear(_renderer);
    }
    
    SDL_SetRenderTarget(_renderer, NULL);
    return texture;
}

void Canvas::GenerateObstacle() {
    srand(time(NULL));
    PerlinNoise perlin( 1 / ((float)rand() + 1), 5);
    float x_cell = (rand() % _dim.w + 1);
    float y_cell = (rand() % _dim.h + 1);

    int width = (rand() % 20 + 10) * 10; width = std::fmin(width, _dim.w - x_cell);
    int height = (rand() % 20 + 10) * 10; height = std::fmin(height, _dim.w - y_cell);

    SDL_Texture * obstacleTexture = createTexture({width, height}, &DARK_GREY, &SOFT_WHITE);
    Obstacle * ob = new Obstacle({x_cell, y_cell}, {width, height}, obstacleTexture);
    obstacles.push_back(ob);
}

Vehicle * Canvas::GenerateCar (dimension d) {
    SDL_Texture * carTexture = createTexture(d, &SOFT_WHITE);
    Vehicle * car = new Vehicle({100, 100}, d, carTexture);
    return car;
}

void Canvas::handleEvent (SDL_Event & e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        switch( e.key.keysym.sym ) {
            case 13: GenerateObstacle();
        }
    }
    for (auto& ob : obstacles) ob->handleEvent(e);
    for (auto& ob : obstacles) ob->drag();
}