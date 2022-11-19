#include <iostream>
#include <string>
#include "SDL.h"
#include <cmath>

#include "graphics/color.hpp"
#include "graphics/canvas.h"

Canvas::Canvas(const char * name, dimension d) : _dim(d){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE)) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
    _window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, d.w, d.h, 0);
    _renderer = SDL_CreateRenderer(_window, -1,  SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    _background = drawGrid(d, &SOFT_BLACK, &DARK_GREY, 50);
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
    for (int i = 0; i <= d.w; i+=size) SDL_RenderDrawLine(_renderer, i, 0, i, d.h); // verticle
    for (int i = 0; i <= d.h; i+=size) SDL_RenderDrawLine(_renderer, 0, i, d.w, i); // horizontal

    SDL_SetRenderTarget(_renderer, NULL);
    return grid;
}

void Canvas::clear() {
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _background, NULL, NULL);
}

void Canvas::present() {
    SDL_RenderPresent(_renderer);
}

void Canvas::render(Object & obj) {
    SDL_Rect src = {0, 0, obj.getDimension().w, obj.getDimension().h};
    SDL_Rect dst = {100, 100, obj.getDimension().w, obj.getDimension().h};
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); 
    SDL_RenderCopy(_renderer, obj.getTexture(), &src, &dst);
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