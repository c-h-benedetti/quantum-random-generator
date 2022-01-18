#include <cassert>
#include "UniformDistribution.hpp"

void UniformDistribution::apply_to_uniforms(std::vector<float>& data){
    for (float& f : data){
        f *= this->range;
        f += range_min;
    }
}

void UniformDistribution::generate_cdf(){}

UniformDistribution::UniformDistribution(const float& r_min, const float& r_max): range_min(r_min), range_max(r_max){
    assert(this->range_min < this->range_max);
    this->range = range_max - range_min;
}

UniformDistribution::~UniformDistribution(){}