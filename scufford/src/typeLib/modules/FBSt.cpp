#include "FBSt.h"

void FBSt::execute(GlobalOutputs* outputs){
    for(const auto p:inputs){
        std::cout << p.first << ": " << p.second << " ST"<<std::endl;
    }
}