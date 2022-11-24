// include c++ libs
#include <iostream>
#include <string>
#include "SDL.h"
#include <cmath>
#include <memory>
#ifdef USE_EMSDK
#include <emscripten.h>
#endif

#include "core/engine.h"

#define SCREEN_FPS 120.0

// public vars
std::unique_ptr<Engine> mainEngine;
static bool is_running = true;

// separate main_loop from main for emscripten
void main_loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {is_running = false;}
        if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) {is_running = false;}
        mainEngine->handleEvent(event);
    }
    mainEngine->run();
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_EVENTS)) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }

    // ScreenDim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    mainEngine = std::unique_ptr<Engine>(new Engine("Simulator"));

#ifdef USE_EMSDK
    emscripten_set_main_loop(main_loop, SCREEN_FPS, 1);
#else
    const float frame_period = (1.0f / SCREEN_FPS) * 1000.0f; 
    // main loop
    while (is_running) {
        Uint64 t_frame_start = SDL_GetPerformanceCounter();

        main_loop();

        Uint64 t_frame_end = SDL_GetPerformanceCounter();

        // cap fps
        float t_elapsed = (t_frame_end - t_frame_start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        if (std::floor(frame_period - t_elapsed) >= 0)
            SDL_Delay(std::floor(frame_period - t_elapsed));
    }
#endif

    // deallocate
    SDL_Quit();
}