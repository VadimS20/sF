#pragma once
#include "../IFB.h"

class FBSumOfTwo : public IFB{
private:
    void execute(GlobalOutputs* outputs) override;
public:
    FBSumOfTwo(std::map<std::string, std::string> inputs,
        std::map<std::string, std::string> connections, 
        std::vector<std::string> next,
        std::string FBname) : IFB(inputs, connections, next, FBname) {};
};