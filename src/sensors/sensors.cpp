#include "sensors/sensors.h"

Camera::Camera (position center, float centralAngle, float fov) {
    float start = centralAngle - fov / 2;
    float end = centralAngle + fov / 2;
    for (float i = start; i <= end; i+=0.2) {
        rays.push_back(new Ray(center, i, false));
    }
}

void Camera::updateState (position ori, angle a) {
    for (auto & ray : rays) {
        ray->_origin = ori;
        ray->_rot = std::fmod(ray->_rot + a, 360);
    }
}

RayDar::RayDar (position center) {
    for (float i = 0; i <= 359; i+=1) {
        rays.push_back(new Ray(center, i, true));
    }
}

void RayDar::updateState(position ori) {
    for (auto & ray : rays) {
        ray->_origin = ori;
    }
}