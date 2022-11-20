#include <iostream>
#include <string>
#include <SDL.h>
#include <cmath>

#include "core/engine.h"

Engine::Engine(const char * name, dimension d) : _dim(d){
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_EVENTS)) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
    _window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, d.w, d.h, 0);
    _renderer = SDL_CreateRenderer(_window, -1,  SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    _background = drawGrid(d, &SOFT_BLACK, &DARK_GREY, 40);
    car = GenerateCar({20, 20});
}

Engine::~Engine() {
    SDL_DestroyTexture(_background);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

SDL_Texture * Engine::drawGrid (dimension d, const SDL_Color * bc, const SDL_Color * gc, int size) {
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
    SDL_SetRenderDrawColor(_renderer, SOFT_WHITE.r, SOFT_WHITE.g, SOFT_WHITE.b, SOFT_WHITE.a); 
    SDL_Rect border = {0, 0, d.w, d.h};
    SDL_RenderDrawRect(_renderer, &border);

    SDL_SetRenderTarget(_renderer, NULL);
    return grid;
}

SDL_Texture * Engine::createTexture(dimension d, const SDL_Color * color, const SDL_Color * borderColor) {
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

void Engine::GenerateObstacle() {
    srand(time(NULL));
    float x_cell = (rand() % _dim.w + 1);
    float y_cell = (rand() % _dim.h + 1);

    int width = (rand() % 20 + 10) * 10; width = std::fmin(width, _dim.w - x_cell);
    int height = (rand() % 20 + 10) * 10; height = std::fmin(height, _dim.w - y_cell);

    SDL_Texture * obstacleTexture = createTexture({width, height}, &DARK_GREY, &SOFT_WHITE);
    Obstacle * ob = new Obstacle({x_cell, y_cell}, {width, height}, obstacleTexture);
    obstacles.push_back(ob);
}

Vehicle * Engine::GenerateCar (dimension d) {
    SDL_Texture * carTexture = createTexture(d, &SOFT_WHITE);
    Vehicle * car = new Vehicle({100, 100}, d, carTexture);
    return car;
}

void Engine::run() {
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _background, NULL, NULL);
    for (auto ob : obstacles) render(ob);

    handleCarMove(car);
    render(car);

    SDL_RenderPresent(_renderer);
}

void Engine::render(Object * obj, SDL_Texture * texture) {
    SDL_Rect src = {0, 0, obj->getDim().w, obj->getDim().h};
    SDL_FRect dst = {obj->getPos().x, obj->getPos().y, (float)obj->getDim().w, (float)obj->getDim().h};
    if (texture) SDL_SetRenderTarget(_renderer, texture);
    

    if (obj->getAngle() != 0) {
        float x_center = (obj->getDim().w / 2) ;
        float y_center = (obj->getDim().h / 2) ;
        SDL_FPoint center = {x_center, y_center};
        SDL_RenderCopyExF( _renderer, obj->getTexture(), &src, &dst, obj->getAngle(), &center, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyF(_renderer, obj->getTexture(), &src, &dst);
    }

    if (texture) SDL_SetRenderTarget(_renderer, NULL);
}

void Engine::handleEvent (SDL_Event & e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        switch( e.key.keysym.sym ) {
            case 13: GenerateObstacle();
        }
    }
    for (auto& ob : obstacles) {ob->handleEvent(e);ob->drag();}
    car->handleEvent(e); 
}

void Engine::handleCarMove (Vehicle * v) {
    float x_offset = v->getVel().x;
    float y_offset = v->getVel().y;

    SDL_FRect v_collider = {v->getPos().x+x_offset, v->getPos().y+y_offset, v->getDim().w, v->getDim().h};
    for (auto & ob : obstacles) {
        SDL_FRect ob_collider = {ob->getPos().x, ob->getPos().y, ob->getDim().w, ob->getDim().h};
        SDL_FRect temp; 
        if (SDL_IntersectFRect(&v_collider, &ob_collider, &temp)) { 
            v->getPos() = {v->getPos().x-x_offset,v->getPos().y-y_offset};
            break;
        }
    }
    car->move();
}

