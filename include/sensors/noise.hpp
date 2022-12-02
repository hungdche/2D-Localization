#pragma once
#include <random>

#include "core/physics.hpp"

struct Noise {
    Noise() {
        std::random_device rd;
        auto _gen = std::mt19937(rd());


    }
};