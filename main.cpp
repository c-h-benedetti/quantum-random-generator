#include <iostream>
#include <cpr/cpr.h>
#include <string>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <stdio.h>
#include <future>
#include <chrono>
#include <cmath>
#include <exception>
#include <assert.h>
#include "RandConfig.h"


#define SIZE_SEGMENT 1024
#define ASYNC_ACTIVE 1
#define DEBUG 1

struct NoConnectionException : public std::exception {
   const char * what () const throw () {
      return "No internet connection, http request failed";
   }
};

static std::mutex mutex_seg_random;

static void feed(std::vector<float>* principal, u_int segment){

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    ACQUIRING DATA FROM ANU QRNG                                       ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    // Transforming size in string to compose the request URL.
    char buffer [16];
    snprintf(buffer, 16, "%d", segment);
    std::string length = buffer;

    cpr::Response r = cpr::Get(cpr::Url{"https://qrng.anu.edu.au/API/jsonI.php?length=" + length + "&type=uint16"});

    int error = r.status_code;
    if(!error){
        throw NoConnectionException{};
    }

    std::string str = r.text;

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    PREFORMATING DATA BEFORE CAST                                      ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    std::size_t found_first = str.find('['); // Begining of the javascript array
    std::size_t found_last = str.rfind(']'); // Ending of the javascript array

    std::size_t size_substr = found_last - found_first - 1;

    std::string data = str.substr(found_first+1, size_substr);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    PARSING DATA INTO NUMBERS                                          ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    std::stringstream check1(data);
    std::string intermediate;
    std::vector<float> tokens;

    // Tokenizing
    while(getline(check1, intermediate, ','))
    {
        tokens.push_back(std::atof(intermediate.c_str()) / 65535.0f);
    }

    std::lock_guard<std::mutex> lock(mutex_seg_random);
    principal->insert(principal->end(), tokens.begin(), tokens.end());

}


class Distribution{

    public:
        
        virtual void apply_to_uniforms(std::vector<float>& data){
            return;
        }

    protected:

        virtual void generate_cdf(){}

        float* cdf = nullptr;
};



class UniformDistribution : public Distribution{
    
};



float normal_distrib(float x, float moyenne=0.0, float std_dev=1.0){
    float coef = 1.0 / (2.50662827463 * std_dev);
    float expo = std::exp(-0.5 * pow(((x - moyenne) / std_dev), 2.0));
    return coef * expo;
}

class NormalDistribution : public Distribution{
    public:
        NormalDistribution(float avg=0.0, float std_dev=1.0, int nb_samples=80, float minBound = -1.0, float maxBound = 1.0){
            this->nb_samples = nb_samples;
            this->average = avg;
            this->standard_deviation = std_dev;
            this->min_boundary = minBound;
            this->max_boundary = maxBound;
            this->generate_cdf();
        }

        void apply_to_uniforms(std::vector<float>& data) override{
            for(float& f : data){
                float *ptr = std::lower_bound(cdf, cdf + nb_samples + 1, f);
                int off = (int)(ptr - cdf - 1);
                float t = (f - cdf[off]) / (cdf[off + 1] - cdf[off]);
                float x = (off + t) / (float)(nb_samples); 
                f = min_boundary + (max_boundary - min_boundary) * x;
            }
        }

    private:

        void generate_cdf() override{
            float cdf[nb_samples + 1], dx = (max_boundary - min_boundary) / (float)nb_samples, sum = 0.0f; 
            cdf[0] = 0.0f;
            cdf[nb_samples] = 1.0f;

            for (int n = 1; n < nb_samples; ++n) { 
                float x = min_boundary + (max_boundary - min_boundary) * (n / (float)(nb_samples)); 
                float pdf_x = normal_distrib(x, average, standard_deviation) * dx; 
                cdf[n] = cdf[n - 1] + pdf_x; 
                sum += pdf_x; 
            }
            
            assert(sum > 0.99);

            this->cdf = cdf;
        }

    
    private:
        int nb_samples;
        float average;
        float standard_deviation;
        float min_boundary;
        float max_boundary;
};


class QuantumRandom{

    std::vector<float> data;
    u_int max_size = 0;
    std::vector< std::future<void> > tasks;
    Distribution distri;

    public:

        QuantumRandom(){
        }

        // Makes enough requests to get an array of "sz" random numbers
        QuantumRandom(u_int sz){
            this->max_size = sz;
            this->generate();
        }

        QuantumRandom(u_int sz, Distribution& d){
            this->max_size = sz;
            this->generate();
            d.apply_to_uniforms(this->data);
        }

        const std::vector<float>& get_data() const{
            return this->data;
        }

        float& operator[](u_int i){
            return this->data[i];
        }

    private:

        void generate(){

            u_int iterations = this->max_size / SIZE_SEGMENT;
            u_int extra = this->max_size % SIZE_SEGMENT;

            #if ASYNC_ACTIVE

            for(u_int i = 0 ; i < iterations ; i++){
                this->tasks.push_back(std::async(std::launch::async, feed, &this->data, SIZE_SEGMENT));
            }
            if(extra){
                this->tasks.push_back(std::async(std::launch::async, feed, &this->data, extra));
            }

            for(std::future<void>& f : this->tasks){
                f.wait();
            }

            #else

            for(u_int i = 0 ; i < iterations ; i++){
                feed(&this->data, SIZE_SEGMENT);
            }
            if(extra){
                feed(&this->data, extra);
            }

            #endif

        }

};


int main(int argc, char* argv[], char* env[]){

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    DISPLAYING VERSION OF THE ALPHA                                    ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    #if DEBUG
    std::cout << "= = = = = = = = = = = = =" << std::endl;
    std::cout << "    Version: " << NaturalRandom_VERSION_MAJOR << "." << NaturalRandom_VERSION_MINOR << std::endl;
    std::cout << "= = = = = = = = = = = = =" << std::endl << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    #endif

    UniformDistribution UD;
    NormalDistribution ND(0.0, 0.35);
    QuantumRandom n(7691);

    #if DEBUG
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << std::endl; 
    #endif

    return 0;
}
