#pragma once 

#include "sensors/ray.hpp"
#include <iomanip> 
#include <random>

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
#pragma endregion

// struct Noise {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     Noise(){};
//     float getNoise (float var) {
//         std::normal_distribution<float> dist(0.0, var);
//         return dist(gen); 
//     }
// };

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