#pragma once
#include <vector>
#include <cmath>

template<typename T>
T RandT(T _min, T _max);

std::vector<float> random_dir();
std::vector<float> random_dir(float theta_min, float theta_max);

