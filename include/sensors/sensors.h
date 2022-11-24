#pragma once 

#include "sensors/ray.hpp"

struct Camera {
    std::vector<Ray *> rays;
    Camera (position center, float centralAngle, float fov);
    void updateState (position ori, angle a);
};

struct RayDar {
    std::vector<Ray *> rays;
    RayDar (position center);
    void updateState (position ori);
};