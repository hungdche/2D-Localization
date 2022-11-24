# 2D Localization Simulator

This is an interactive simulation to visualize 2D Simulation using SDL graphic library.  

### Prerequisites

1. SDL2 Version at least 2.0.20
2. Emscripten (Optional)

### Installation 

1. clone the repo 

        $ git clone 
        $ mkdir build ; cd build 

1. Normal build 

        $ cmake .. 
        $ make 
        $ ./simulator

2. WebAssembly 

        $ emcmake -DUSE_EMSDK=ON ..
        $ emmake make
        $ emrun simulator.html