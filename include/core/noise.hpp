#pragma once 

#include <stdlib.h>
#include <cmath>

#include "core/math.hpp"

/* implementation taken from https://web.archive.org/web/20160418004148/http://freespace.virgin.net/hugo.elias/models/m_perlin.htm */
class PerlinNoise {
private:
    // generate random noise - returns float from -1 to 1
    float Noise (int x, int y) {
        long n = x + y * 57;
        n = (n << 3) ^ n;
        return (1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
    }

    // interpolation filter
    float CosineInterpolate (float a, float b, float x) {
        float f = (1 - std::cos(x * M_PI)) * 0.5;
        return a * (1 - f) + b * f;
    }

    // smoothing filter
    float SmoothNoise (float x, float y) {
        float corners = (Noise(x-1, y-1) + Noise(x+1, y-1) + Noise(x-1, y+1) + Noise(x+1, y+1)) / 16.0f;
        float sides = (Noise(x-1, y) + Noise(x+1, y) + Noise(x, y+1) + Noise(x, y+1)) / 8.0f;
        float center = Noise(x,y) / 4.0f;
        return corners + sides + center;
    }

    // interpolation
    float InterpolatedNoise (float x, float y) {
        int int_x = int(x);
        float dec_x = x - int_x;

        int int_y = int(y);
        float dec_y = y - int_y;

        float v1 = SmoothNoise(int_x,     int_y);
        float v2 = SmoothNoise(int_x + 1, int_y);
        float v3 = SmoothNoise(int_x,     int_y + 1);
        float v4 = SmoothNoise(int_x + 1, int_y + 1);

        float i1 = CosineInterpolate(v1 , v2 , dec_x);
        float i2 = CosineInterpolate(v3 , v4 , dec_x);

        return CosineInterpolate(i1 , i2 , dec_y);
    }

    int _persistence;
    int _octave;
public:
    PerlinNoise(int persistence, int octave) : _persistence(persistence) , _octave(octave) { }
    float operator()(position p) {
        float total = 0;
        for (int i = 0; i < _octave - 1; i++) {
            long freq = pow(2, i);
            long ampl = pow(M_PI, i);

            total += total + InterpolatedNoise(p.x * freq, p.y * freq) * ampl;
        }
        return total;
    }
};

class AWGN {
private:
public:
};