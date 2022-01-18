#include <iostream>
#include "RandConfig.h"
#include "QuantumRandom.hpp"



int main(int argc, char* argv[], char* env[]){

    // UniformDistribution UD{0.0, 100.0};
    // NormalDistribution ND{0.0, 3.0, -10.0, 10.0};
    ExponentialDistribution ED{1.0};
    
    QuantumRandom n{&ED};

    
    for (u_int i = 0 ; i < 50 ; i++){
        std::cout << n.get() << std::endl;
    }


    return 0;
}
