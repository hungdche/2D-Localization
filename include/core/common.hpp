#pragma once

#include <math.h>
#include <iostream>
#include <SDL.h>
#include <vector>

// structs
struct position { float x; float y; };
struct line { position s; position e;};
struct velocity { float x; float y; };
struct acceleration { float x; float y; };
typedef float angle;
struct dimension { int w; int h; };

// global variable 
extern dimension ScreenDim;

// color
const SDL_Color SOFT_BLACK = {24, 26, 24, 255};
const SDL_Color DARK_GREY = {35, 35, 35, 255};
const SDL_Color SOFT_WHITE = {100, 100, 100, 255};