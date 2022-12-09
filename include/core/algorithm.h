#pragma once

#include <Eigen/Dense>
#include <random>

#include "core/physics.hpp"
#include "sensors/sensors.h"

class Algorithm {
protected:
    Eigen::Vector3f x;
    Algorithm(position pos, angle a) : x(pos.x, pos.y, a) { };

public:
    virtual void feedPose(control data) = 0;
    state dumpPose() { 
        position pos = {x[0], x[1]};
        return {pos, x[2]}; 
    };
    virtual ~Algorithm() {};
};

class EKF : public Algorithm {
private:
    /* PREDICTION */

    // x' = F.x + Bu + v
    Eigen::Matrix3f F; // state transition matrix <- identity 
    Eigen::Matrix<float,3,2> B; // control input matrix

    // P' = FPF^T + Q
    Eigen::Matrix3f P; // Predicted Covariance Estimate
    Eigen::Matrix3f Q; // covariance matrix of motion disturbance

    /* UPDATE */

    // y = z - h(x')
    // h = H.x' + w
    // S = HP'H^T + R
    Eigen::Matrix3f H; // measurement matrix usually the Jacobian 
    Eigen::Matrix3f R; // covariance matrix of sensor noise 

    // K = P'(H^T)S^(-1)
    float K; // Kalman Gain
public:
    EKF(position pos, angle a, velocity lin, velocity ang);
    ~EKF();

    void feedPose(control data);
};
