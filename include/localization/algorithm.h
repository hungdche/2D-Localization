#pragma once

#include <Eigen/Dense>
#include <random>
#include <chrono>
#include <thread>

#include "physics.hpp"
#include "sensors.hpp"

class EKF{
private:
    /* state */
    Eigen::Vector3f x;                  // state vector
    Eigen::Vector2f u;                  // control vector

    /* noises */
    Eigen::Matrix3f Q;                  // covariance matrix of motion disturbance
    Eigen::Matrix3f R;                  // covariance matrix of sensor noise 

    /* prediction */
    Eigen::Matrix3f F;                  // state transition matrix <- identity 
    // Eigen::Matrix<float,3,2> B;         // control input matrix
    Eigen::Matrix3f P;                  // Predicted Covariance Estimate
    
    Eigen::Vector3f x_hat;              // predicted state
    Eigen::Matrix3f P_hat;             

    /* measurement */
    Eigen::Matrix3f H;                  // measurement matrix usually the Jacobian 
    Eigen::Matrix3f S;
    Eigen::Matrix3f K;                  // Kalman Gain

public:
    EKF(state s, Eigen::Matrix3f q, Eigen::Matrix3f r);
    ~EKF();

    state predict(control data);
    void update(state s);
    
    state dumpPose() { 
        position pos = {x[0], x[1]};
        return {pos, x[2]}; 
    };
};
