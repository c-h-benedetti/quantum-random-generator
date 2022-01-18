#ifndef UNIFORM_DISTRIBUTION_BASE_HPP_INCLUDED
#define UNIFORM_DISTRIBUTION_BASE_HPP_INCLUDED

#include "Distribution.hpp"

class UniformDistribution : public Distribution{
    
private:

    float range_min = 0.0;
    float range_max = 1.0;
    float range = 0.0;

public:

    void apply_to_uniforms(std::vector<float>& data) override;
    void generate_cdf() override;

    UniformDistribution(const float& r_min=0.0, const float& r_max=1.0);

    ~UniformDistribution();
};


#endif // UNIFORM_DISTRIBUTION_BASE_HPP_INCLUDED