
#pragma once

#include <fstream>
#include <Eigen/Dense>
#include <string>
#include "yaml-cpp/yaml.h"

inline void operator>> (const YAML::Node& node, Eigen::Matrix3f& v) {
    v(0,0) = node[0][0].as<float>(); v(0,1) = node[0][1].as<float>(); v(0,2) = node[0][2].as<float>();
    v(1,0) = node[1][0].as<float>(); v(1,1) = node[1][1].as<float>(); v(1,2) = node[1][2].as<float>();
    v(2,0) = node[2][0].as<float>(); v(2,1) = node[2][1].as<float>(); v(2,2) = node[2][2].as<float>();
}


struct Parser {
    float max_speed;

    float noise_a; 
    float bias_a; 

    float noise_g;
    float bias_g;

    Eigen::Matrix3f Q;
    Eigen::Matrix3f R;

    
    Parser(std::string str) {
        YAML::Node doc = YAML::LoadFile(str);

        const YAML::Node& car = doc["car"];
        max_speed = car["max_speed"].as<float>();

        const YAML::Node& imu = doc["imu"];
        noise_a = imu["accelerometer_noise_density"].as<float>();
        bias_a = imu["accelerometer_random_walk"].as<float>();
        noise_g = imu["gyroscope_noise_density"].as<float>();
        bias_g = imu["gyroscope_random_walk"].as<float>();

        const YAML::Node& ekf = doc["ekf"];
        ekf["measurement_covariance_matrix"] >> R;
        ekf["prediction_covariance_matrix"] >> Q;

    }
};