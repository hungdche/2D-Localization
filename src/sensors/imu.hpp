#include "sensors/sensors.hpp"

class IMU {
private:
    float noise_a;
    float noise_g;
    float bias_a;
    float bias_g;
    Noise noise_model;

public:
    IMU(float n_a, float n_g, float b_a, float b_g) 
        : noise_a(n_a), noise_g(n_g), bias_a(b_a), bias_g(b_g) {}

    ~IMU() {}

    
    
}