#include "FBConsoleOut.h"
#include <chrono>
#include <thread>

void FBConsoleOut::execute(GlobalOutputs* outputs){
    for(const auto p:inputs){
        std::cout << p.first << ": " << p.second << std::endl;
        
    }
}