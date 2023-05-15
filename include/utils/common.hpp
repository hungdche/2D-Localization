#pragma once

#include <math.h>
#include <iostream>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "types.hpp"

// global variable 
extern dimension ScreenDim;

// color
const SDL_Color SOFT_BLACK = {24, 26, 24, 255};
const SDL_Color DARK_GREY = {35, 35, 35, 255};
const SDL_Color SOFT_WHITE = {100, 100, 100, 255};
const SDL_Color DARK_BLUE = {23, 48, 67, 255};
const SDL_Color LIGHT_BLUE = {118, 194, 227, 255};
const SDL_Color NEON_BLUE = {12, 141, 255, 255};
const SDL_Color WHITE = {150, 150, 150, 255};
const SDL_Color BLACK = {0, 0, 0, 255};