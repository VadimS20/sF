#pragma once

#include <map>
#include <string>


class GlobalOutputs {
private:
    static GlobalOutputs* instance;
    std::map<std::string, std::string> outputs;
    GlobalOutputs(std::map<std::string, std::string> outputs) : outputs(outputs) {}

public:
    static GlobalOutputs* getInstance(std::map<std::string, std::string> outputs);
    std::map<std::string, std::string> getOutputs();
    void setOutputs(std::map<std::string, std::string> outputs);
    void setOutput(std::string name, std::string value);
    void clear();
};
