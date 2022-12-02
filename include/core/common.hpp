#pragma once

#include <math.h>
#include <iostream>
#include <SDL.h>
#include <vector>
#include <memory>

// state
struct position { 
    float x; 
    float y;
    bool operator==(const position & other) { 
        return (x == other.x && y == other.y);
    };
};

enum is_accelerate {non, positive, negative, deccelerate};
typedef float velocity;
typedef float acceleration;
typedef float angle;

// geometry
struct dimension { int w; int h; };
struct line { position s; position e;};

// global variable 
extern dimension ScreenDim;

// color
const SDL_Color SOFT_BLACK = {24, 26, 24, 255};
const SDL_Color DARK_GREY = {35, 35, 35, 255};
const SDL_Color SOFT_WHITE = {100, 100, 100, 255};
const SDL_Color DARK_BLUE = {23, 48, 67, 255};
const SDL_Color LIGHT_BLUE = {118, 194, 227, 255};
const SDL_Color NEON_BLUE = {12, 141, 255, 255};