# 2D Localization

This is an interactive simulation to visualize 2D Simulation using SDL graphic library. This project was developed mainly as a way for me to wrap my head around different techiniques in robotic localization, and what is more beginner-friendly that starting things off in 2D? 

I have also compiled this project into WebAssembly and deployed it on Github, which is available here: [hungdche.github.io/2D-Localization](https://hungdche.github.io/2D-Localization/)


<p>
  <img style="width: 100%;" src="screenshots/sim.png" />
</p>

## Prerequisites
* **SDL2**: Make sure to have installed SDL2 Version at least 2.0.20. Visit https://wiki.libsdl.org/SDL2/Installation for installation instruction. It is recommded that you install from source instead of `libsdl2-dev` from `apt`, since SDL version on apt is 2.0.10.
* **Eigen**: If you want to build with Emscripten, clone this repo recursively.
* **yaml-cpp**: Instruction on how to download is here: https://github.com/jbeder/yaml-cpp

## Installation 


        git clone https://github.com/hungdche/Localization-Simulation.git
        cd 2D-Localization
        mkdir build && cd build 
        cmake ..
        make -j

## Features

In this simulation, a vehicle (grey square). is equipped with a camera, IMU, and Lidar. To replicate the real world scenario, IMU and Lidar data is infused with additive zero-mean Gaussian noise, modal as below. (Camera are mostly for direction visualization, and does not affect localization).  
The task is to use each or a combination of sensors to try and localize the vehicle using Kalman Filter and its derivative. The cyan square indicated the estimated pose of the vehicle. 

The basic movements are:

* `Left, Right button`: steer left/right
* `Up button` to accelerate in the positive direction
* `Down button` to accelerate in the negative direction 
* `n` to reset the predicted pose
* `l` to switch between editting mode and simulating mode
* `Space` to brake
* `Enter` to randomly spawn obstacles of random sizes, can be drag around  

You can specify things like max speed, noises and biases for sensors, and noise covariance matrices in `configs/config.yaml`. You can add more parameters by modifying `include/core/parser.hpp`
    


## Method

The state vector that we always want to estimate is $\mathbf{x} = \left[ x,y,\theta \right]^T$, where $[x,y]$ is the location, and $\theta$ is the angle with respect to the y axis.  

### IMU Simulation Model 

To try and replicate real-world IMU data, both the accelerometer and gyroscope are influenced by additive white noise and random walk. 

* $\tilde{a}(t) = a(t) + b_a(t) + n_a(t)$ and $\tilde{w}(t) = w(t) + b_w(t) + n_w(t)$ 
  * noise can be simulated as follows: $n[k] = \sigma_{d}w[k]$ 
    * where $w[k] \sim \mathcal{N}(0,1)$ and $\sigma_{d} = \sigma \dfrac{1}{\sqrt{\Delta t}}$
  * bias can be simulated as follows: $b[k] = b[k-1] + \sigma_{d}w[k]$ 
    * where $w[k] \sim \mathcal{N}(0,1)$ and $\sigma_{d} = \sigma \sqrt{\Delta t}$

For more information, refer here:
* https://github.com/ethz-asl/kalibr/wiki/IMU-Noise-Model
* https://www.mathworks.com/help/nav/ref/imusensor-system-object.html
