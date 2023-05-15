#pragma once 

#include <iostream>
#include <random>
#include <iomanip>


struct position { 
    float x; 
    float y;
    bool operator==(const position & other) { 
        return (x == other.x && y == other.y);
    };
    int direction(const position & other) {
        if (x == other.x) return 0;
        if (x == other.y) return 1;
    }
};

enum is_accelerate {non, positive, negative, deccelerate};
typedef float velocity;
typedef float acceleration;
typedef float angle;

// geometry
struct dimension { int w; int h; };
struct line { position s; position e;};

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
