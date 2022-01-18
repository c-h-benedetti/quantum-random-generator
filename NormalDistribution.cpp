#include "NormalDistribution.hpp"


float normal_distrib(float x, float moyenne, float std_dev){
    float coef = 1.0 / (2.50662827463 * std_dev);
    float expo = std::exp(-0.5 * pow(((x - moyenne) / std_dev), 2.0));
    return coef * expo;
}


NormalDistribution::NormalDistribution(float avg, float std_dev, float minBound, float maxBound){
    this->average = avg;
    this->standard_deviation = std_dev;
    this->min_boundary = minBound;
    this->max_boundary = maxBound;
    this->generate_cdf();
}


NormalDistribution::~NormalDistribution(){
    delete[] this->cdf;
}


void NormalDistribution::apply_to_uniforms(std::vector<float>& data){
    for(float& f : data){
        float *ptr = std::lower_bound(cdf, cdf + NB_SAMPLES + 1, f);
        int off = (int)(ptr - cdf - 1);
        float t = (f - cdf[off]) / (cdf[off + 1] - cdf[off]);
        float x = (off + t) / (float)(NB_SAMPLES); 
        f = min_boundary + (max_boundary - min_boundary) * x;
    }
}




void NormalDistribution::generate_cdf(){
    float* cdf = new float [NB_SAMPLES + 1];
    float dx = (max_boundary - min_boundary) / (float)NB_SAMPLES; // increment on horizontal axis

    cdf[0] = 0.0f;
    cdf[NB_SAMPLES] = 1.0f;

    for (size_t n = 1 ; n < NB_SAMPLES ; n++) { 
        float x = min_boundary + n * dx; 
        float pdf_x = normal_distrib(x, average, standard_deviation) * dx; // Multiply by 'dx' to bind the value between 0 and 1.
        cdf[n] = cdf[n - 1] + pdf_x;
    }
    
    assert(cdf[NB_SAMPLES-1] > 0.99);
    this->cdf = cdf;
}

