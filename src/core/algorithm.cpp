#include "core/algorithm.h"

EKF::EKF(position pos, angle a, velocity lin, velocity ang) : Algorithm(pos, a){
    // initialize shit here
}

EKF::~EKF() { }

void EKF::feedPose(control data) {
    Eigen::Vector2f u(data._vel, data._yaw);
    
    /* PREDICTION */
    Eigen::Vector3f x_hat = F * x + B * u;
    
}


/*
        1. Initialization 
            init location and control inputs (linear a and ang a)
        2. Predicted State Estimate - State Space model 
            x_i = x_i-1 + u_i-1 + noise 
        3. Predicted Covariance of the State Estimate 
            P_(k|k-1) = F_k * P_(k-1|k-1) * F^T_k + Q_k
                . P_(k-1|k-1) : square mat with same number of row as the state 
                . F_k and F^T_k : Jacobian Mat <- state space modeling 
                . Q_k : noise covariance matrix
                    [ Cov(x,x)   Cov(x,y)   Cov(x,yaw)
                      Cov(y,x)   Cov(y,y)   Cov(y,yaw)
                      Cov(yaw,x) Cov(yaw,y) Cov(yaw,yaw) ]
        4. Calculate dif between actual sensor observations and predicted sensor observations <- observation model 
            y_i = z_i - h(x_(k|k-1)
                . z_i is actual reading from the sensors 
                . h( x_(k|k-1) ) = H_k * x_(k|k-1) + w_k
                    . H_k is estimation matrix 
                    . x_(k|k-1) is predicted state from step 2
                    . w_k is sensor noise assumption
        5. Innovation or residual covariance 
            S_(k|k-1) = H_k * P_(k-1|k-1) * H^T_k + R_k
        6. Near Optimal Kalman Gain 
            K_k = P_(k|k-1) * H^T_k * S^(-1)_k
                . P_(k|k-1): from  step 3
                . H^T_k: measurement matrix
                . S^(-1)_k: from step 5
            -> indicate how much stat and covariance should be corrected

        7. Update state estimate 
            x_(k|k) = x_(k|k-1) + K_k*y_k
        8. Update Covar of the State Estimate 
            P_(k|k) = (I - K_k * H_k) * P_(k|k-1)
    */