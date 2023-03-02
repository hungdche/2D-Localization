#include "core/algorithm.h"

EKF::EKF(state s) {
    x << s._pos.x, s._pos.y, s._rot;
    F = Eigen::Matrix3f::Identity();
    P = Eigen::Matrix3f::Identity();
    H = Eigen::Matrix3f::Identity();
    Q = Eigen::Matrix3f::Identity() ;
    R = Eigen::Matrix3f::Identity() ;
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
    // /* UPDARTE */
    x = x_hat + K * y;

    // std::cout << P - P_hat << std::endl;
    
    P = (Eigen::Matrix3f::Identity() - (K * H)) * P_hat;
}

#ifdef TEST_EKF

int main(int argc, char *argv[]) {
    state true_state = {{0,0}, 0};
    EKF * e = new EKF(true_state);
    
    control con = {1,1,0};

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::normal_distribution<float> dist(0.0, 0.5); 
    std::normal_distribution<float> dist2(0.0, 0.5); 

    int iteration = 1;
    while(1) {
        std::cout << "Iteration " << iteration << std::endl;
        true_state.increment();

        control noisy_control = {dist(gen), dist(gen), 0};
        state noisy_measure = true_state.add({{dist(gen), dist(gen)}, 0});

        state predicted = e->predict(noisy_control);
        e->update(noisy_measure);
        state est = e->dumpPose();
        std::cout << "True: " << true_state << std::endl;
        std::cout << "Nois: " << noisy_measure << std::endl;
        std::cout << "Pred: " << predicted << std::endl;
        std::cout << "Est : " << est << std::endl;
        std::cout << "_______" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        iteration++;
        
    }
}

#endif