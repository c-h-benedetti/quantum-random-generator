#ifndef EXPONENTIAL_DISTRIBUTION_HPP_INCLUDED
#define EXPONENTIAL_DISTRIBUTION_HPP_INCLUDED

#include "Distribution.hpp"

class ExponentialDistribution : public Distribution{

public:

    void apply_to_uniforms(std::vector<float>& data) override;
    void generate_cdf() override;
    
    ExponentialDistribution(float l=1.0);

    ~ExponentialDistribution();
    
private:

    float lambda;
};

#endif // EXPONENTIAL_DISTRIBUTION_HPP_INCLUDED