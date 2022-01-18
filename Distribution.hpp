#ifndef DISTRIBUTION_BASE_HPP_INCLUDED
#define DISTRIBUTION_BASE_HPP_INCLUDED

#include <vector>

#define MAX_FEED 1024

class Distribution{

    public:
        
        Distribution(){}
        virtual void apply_to_uniforms(std::vector<float>& data) = 0;
        virtual ~Distribution(){}

    protected:

        virtual void generate_cdf() = 0;
        float* cdf = nullptr;
};

#endif // DISTRIBUTION_BASE_HPP_INCLUDED