#pragma once

#include <math.h>
#include <iostream>

// structs
struct position { float x; float y; };
struct velocity { float x; float y; };
struct acceleration { float x; float y; };
struct rotation { float angle; };
struct dimension { int w; int h; };


// global variable 
extern dimension ScreenDim;
