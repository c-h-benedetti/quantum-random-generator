#include <exception>
#include <cpr/cpr.h>
#include <string>
#include <sstream>
#include "DataCollector.hpp"


struct NoConnectionException : public std::exception {
   const char * what () const throw () {
      return "No internet connection, HTTP request failed";
   }
};



static std::mutex mutex_seg_random;



void feed(std::vector<float>* principal, u_int segment){

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    ACQUIRING DATA FROM ANU QRNG                                       ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    // Transforming size in string to compose the request URL.
    u_int size_buffer = 256;
    char buffer[256] = {};
    snprintf(buffer, size_buffer, "http://qrng.anu.edu.au/API/jsonI.php?length=%d&type=uint16", segment);

    cpr::Response r = cpr::Get(
        cpr::Url{buffer},
        cpr::VerifySsl{true}
    );

    if(!r.status_code){
        throw NoConnectionException{};
    }


    

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    PREFORMATING DATA BEFORE CAST                                      ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    std::size_t found_first = r.text.find('['); // Begining of the javascript array
    std::size_t found_last = r.text.rfind(']'); // Ending of the javascript array
    std::size_t size_substr = found_last - found_first - 1;

    std::string data = r.text.substr(found_first+1, size_substr);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // ~    PARSING DATA INTO NUMBERS                                          ~
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    std::stringstream check1(data);
    std::string intermediate;
    std::vector<float> tokens;
    tokens.reserve(segment);
    uint16_t max_type = ~0;

    // Tokenizing
    while(getline(check1, intermediate, ',')){
        tokens.push_back(std::atof(intermediate.c_str()) / (float)max_type);
    }

    std::lock_guard<std::mutex> lock(mutex_seg_random);
    
    principal->clear();
    principal->reserve(segment);
    principal->insert(principal->begin(), tokens.begin(), tokens.end());

}