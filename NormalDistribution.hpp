#ifndef NORMAL_DISTRIBUTION_HPP_INCLUDED
#define NORMAL_DISTRIBUTION_HPP_INCLUDED

#define NB_SAMPLES 256

#include <cmath>
#include <cassert>

#include "Distribution.hpp"

float normal_distrib(float x, float moyenne=0.0, float std_dev=1.0);

class NormalDistribution : public Distribution{

private:

    float average;
    float standard_deviation;
    float min_boundary;
    float max_boundary;


public:

    NormalDistribution(float avg=0.0, float std_dev=1.0, float minBound=-5.0, float maxBound=5.0);

    ~NormalDistribution();

    void apply_to_uniforms(std::vector<float>& data) override;


private:

    void generate_cdf() override;
    
};

#endif // NORMAL_DISTRIBUTION_HPP_INCLUDED