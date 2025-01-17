#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "globalOutputsList.h"


class IFB{
protected:
    std::map<std::string, std::string> inputs;
    GlobalOutputs* outputs;
    std::map<std::string, std::string> connections;
    std::vector<std::string> next;
    std::string pathToESSTEE;
    std::string FBname;

    virtual void execute(GlobalOutputs* outputs) = 0;
    void sentOutputs();
    
public:
    IFB(std::map<std::string, std::string> inputs,
        std::map<std::string, std::string> connections, 
        std::vector<std::string> next,
        std::string FBname);
    void call(GlobalOutputs* outputs);
    void setPathToESSTEE(std::string path);
    void addNext(std::string newNext);
    void setConnections(std::map<std::string,std::string> conn);
    std::vector<std::string> getNext();
    std::map<std::string, std::string> getConnections();
    std::map<std::string, std::string> getInputs();
    std::string getName();
    void setNext(std::vector<std::string> newNext);
    void setInputs(std::map<std::string, std::string> inputs);
};
