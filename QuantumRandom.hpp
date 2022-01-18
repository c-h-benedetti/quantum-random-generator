#ifndef QUANTUM_RANDOM_HPP_INCLUDED
#define QUANTUM_RANDOM_HPP_INCLUDED


#include <thread>
#include <mutex>
#include <cstring>
#include <condition_variable>

#include "DataCollector.hpp"
#include "UniformDistribution.hpp"
#include "NormalDistribution.hpp"
#include "ExponentialDistribution.hpp"


class QuantumRandom{

    std::vector<float> data;
    std::vector<float> buffer;
    Distribution* distri = nullptr;
    size_t pos = 0;
    bool run = true;
    std::thread feeder;
    std::mutex mutex;
    std::condition_variable cond;
    size_t size = 0;

public:

    QuantumRandom(Distribution* d);

    ~QuantumRandom();

    float get();

    friend void fetcher(QuantumRandom* qr);

};

void fetcher(QuantumRandom* qr);

#endif // QUANTUM_RANDOM_HPP_INCLUDED