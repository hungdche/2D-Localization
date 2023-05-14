#include "algorithm.h"

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