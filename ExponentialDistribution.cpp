#include "ExponentialDistribution.hpp"
#include <cmath>

void ExponentialDistribution::apply_to_uniforms(std::vector<float>& data){
    for(float& f : data){
        f = -log(f) / this->lambda;
    }
}

void ExponentialDistribution::generate_cdf(){}

ExponentialDistribution::ExponentialDistribution(float l){
    this->lambda = l;
}

ExponentialDistribution::~ExponentialDistribution(){}