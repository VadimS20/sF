#include "globalOutputsList.h"

GlobalOutputs* GlobalOutputs::instance = nullptr;

GlobalOutputs* GlobalOutputs::getInstance(std::map<std::string, std::string> outputs) {
    if (instance == nullptr) {
        instance = new GlobalOutputs(outputs);
    }
    return instance;
}

std::map<std::string, std::string> GlobalOutputs::getOutputs() {
    return outputs;
}

void GlobalOutputs::setOutputs(std::map<std::string, std::string> outputs) {
    this->outputs = outputs;
}

void GlobalOutputs::setOutput(std::string name, std::string value) {
    this->outputs[name] = value;
}

void GlobalOutputs::clear() {
    this->instance = nullptr;
}