#include "algorithm.h"

EKF::EKF(state s, Eigen::Matrix3f q, Eigen::Matrix3f r) {
    x << s._pos.x, s._pos.y, s._rot;
    F = Eigen::Matrix3f::Identity();
    P = Eigen::Matrix3f::Identity();
    H = Eigen::Matrix3f::Identity();
    Q = q;
    R = r;
    u << 0 , 0;
}

EKF::~EKF() { }

state EKF::predict(control c) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::normal_distribution<float> dist(0.0, 0.5); 
    std::normal_distribution<float> dist2(0.0, 0.005); 

    /* PREDICTION */
    u[0] = c._vel * c.dt; u[1] = c._yaw * c.dt;
    Eigen::Matrix<float,3,2> B;
    B << getX(x[2]), 0, getY(x[2]), 0, 0, 1;
    
    Eigen::Vector3f n;
    n << dist(gen), dist(gen), dist2(gen);
    x_hat = F * x + B * u + n;
    P_hat = F * P * F.transpose() + Q;
    
    return {{x_hat[0], x_hat[1]}, x_hat[2]};
}

void EKF::update(state s) {
    /* MEASUREMENT */
    Eigen::Vector3f z; z << s._pos.x, s._pos.y, s._rot;
    Eigen::Vector3f y = z - H * x_hat;
    
    S = H * P_hat * H.transpose() + R;
    K = P_hat * H.transpose() * S.inverse();
    std::cout << K << std::endl;
    // /* UPDARTE */
    x = x_hat + K * y;

    // std::cout << P - P_hat << std::endl;
    
    P = (Eigen::Matrix3f::Identity() - (K * H)) * P_hat;
}