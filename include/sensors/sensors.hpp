#pragma once 

#include "sensors/ray.hpp"
#include <iomanip> 
#include <random>
#include <math.h> 

#pragma region data_struct
struct control {
    velocity _vel;
    angle _yaw;
    float dt;
};

struct state {
    position _pos;
    angle _rot;
    
    void increment() {
        _pos.x++;
        _pos.y++;
    }

    state add(const state& s) {
        return {{_pos.x + s._pos.x, _pos.y + s._pos.y}, _rot + s._rot};
    }

    friend std::ostream & operator <<( std::ostream& out, const state& B) {
        return out << std::setprecision (4) << std::fixed << "Position: " << B._pos.x << " " << B._pos.y << " Rotation: " << B._rot;
    }
};

struct PtdCld {
    float distance;
    angle degree;
    Uint32 timestamp;
};

struct Noise {
    Noise(){};
    float getNoise (float var) {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::normal_distribution<float> dist(0.0, var);
        float n = dist(gen);
        return n; 
    }
};
#pragma endregion

struct Camera {
    std::vector<Ray *> rays;
    Camera (position center, float centralAngle, float fov) {
        float start = centralAngle - fov / 2;
        float end = centralAngle + fov / 2;
        for (float i = start; i <= end; i+=0.005) {
            rays.push_back(new Ray(center, i, false));
        }
    }
    void updateState (position ori, angle a) {
        for (auto & ray : rays) {
            ray->_origin = ori;
            ray->_rot = std::fmod(ray->_rot + a, 2 * M_PI);
        }
    }
};

struct RayDar {
    std::vector<Ray *> rays;
    RayDar (position center, float distance) {
        for (float i = 0; i <= 2 * M_PI; i+=0.025) {
            rays.push_back(new Ray(center, i, true, distance));
        }
    }
    void updateState(position ori) {
        for (auto & ray : rays) {
            ray->_origin = ori;
        }
    }
};

class IMU {
private:
    // accelerometer
    float noise_a; 
    float bias_a; 
    position pos;
    velocity vel;

    // gyroscope
    float noise_g;
    float bias_g;
    angle rot;

    // noise
    Noise noise_model;

public:
    IMU(position p, float n_a, float b_a, float n_g, float b_g) 
        : noise_a(n_a), noise_g(n_g), bias_a(b_a), bias_g(b_g)
        , pos{p}, vel(0), rot(0) {}

    ~IMU() {}

    void updateState(acceleration a, velocity w, float dt) {
        // gyroscope
        float n = noise_g * (1/sqrt(dt)) * noise_model.getNoise(1);
        float b = bias_g * sqrt(dt) * noise_model.getNoise(1);
        w += n + b;
        rot = radAdd(rot, w * dt);

        // accelerometer
        n = noise_a * (1/sqrt(dt)) * noise_model.getNoise(1);
        b = bias_a * sqrt(dt) * noise_model.getNoise(1);
        a += n + b;
        vel += a * dt;
        pos.x += (vel * dt + 0.5 * a * dt * dt) * getX(rot);
        pos.y += (vel * dt + 0.5 * a * dt * dt) * getY(rot);

    }

    // acceleration getAccel() { return current_accel; }
    // velocity getOmega() { return current_omega; }

    state estimatePose() { return {pos, rot}; };


    
};