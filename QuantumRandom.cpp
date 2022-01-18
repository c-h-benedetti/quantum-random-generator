#include "QuantumRandom.hpp"

void fetcher(QuantumRandom* qr){

    while (qr->run){
        feed(&(qr->buffer), qr->size);
        qr->distri->apply_to_uniforms(qr->buffer);
        std::unique_lock<std::mutex> lock(qr->mutex);
        if (!qr->run){
            return;
        }


        for (u_int i = 0 ; i < qr->size ; i++){
            qr->data[i] = qr->buffer[i];
        }

        lock.unlock();
        qr->cond.notify_one();
    }
}


QuantumRandom::QuantumRandom(Distribution* u){
	const u_int buffer_size = 1024;
    this->data.resize(buffer_size);
    this->buffer.resize(buffer_size);
    this->distri = u;
    this->size = buffer_size;
    this->pos = 0;
    this->feeder = std::thread(fetcher, this);
}


float QuantumRandom::get(){
    if (this->pos == 0){
        std::unique_lock<std::mutex> lock(this->mutex);
        this->cond.wait(lock);
        lock.unlock();
    }

    float k = this->data[this->pos];
    this->pos++;

    if (this->pos == this->size){
        this->pos = 0;
    }

    return k;
}


QuantumRandom::~QuantumRandom(){
    this->run = false;
    this->feeder.join();
}