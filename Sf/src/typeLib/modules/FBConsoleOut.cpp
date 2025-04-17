#include "FBConsoleOut.h"
#include <chrono>
#include <thread>

void FBConsoleOut::execute(GlobalOutputs* outputs){
    for(const auto p:inputs){
        std::cout << p.first << " LOG INFO in thread" << std::this_thread::get_id() << ": " << p.second << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}