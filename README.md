# 2D Localization

This is an interactive simulation to visualize 2D Simulation using SDL graphic library. This project was developed mainly as a way for me to wrap my head around different techiniques in robotic localization, and what is more beginner-friendly that starting things off in 2D? 

I have also compiled this project into WebAssembly and deployed it on Github, which is available here: [hungdche.github.io/2D-Localization](https://hungdche.github.io/2D-Localization/)


<p>
  <img style="width: 100%;" src="screenshots/sim.png" />
</p>

## Features

In this simulation, a vehicle <span style="color:grey">(grey square)</span>. is equipped with a camera and a raydar (ray-dar). Both are infused with additive Gaussian noise as an attempt to replicate real-world sensor noise. 

The task is to use various methods, such as Kalman Filter and its derivative, to try and localize the vehicle. THe <span style="color:cyan">cyan square</span> indicated the estimated pose (x,y,theta) of the vehicle. 

The basic movements are:

* `Left, Right button`: steer left/right
* `Up button` to accelerate in the positive direction 
* `Down button` to accelerate in the negative direction 
* `n` to reset the predicted pose
* `l` to switch between editting mode and simulating mode
* `Space` to brake
* `Enter` to randomly spawn obstacles of random sizes, can be drag around  

## Prerequisites

Make sure to have installed SDL2 Version at least 2.0.20. Visit https://wiki.libsdl.org/SDL2/Installation for installation instruction. It is recommded that you install from source instead of `libsdl2-dev` from `apt`, since SDL version on apt is 2.0.10.

## Installation 


        git clone https://github.com/hungdche/Localization-Simulation.git
        cd 2D-Localization
        mkdir build && cd build 
        cmake ..
        make -j

