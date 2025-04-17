#include "BasicSTFB.h"

void BasicSTFB::execute(GlobalOutputs* outputs){
    std::string command = "./STInterpreter/INTERPRETER --file=\"./STBlocks/" + this->FBname + ".st\" --program=\"execute\" --pre-run-queries=\"";
    int in_num = 1;
    std::string post_querry = "\" --post-run-queries=\"";
    for(const auto p : this->inputs){
        command += "[execute].input" + std::to_string(in_num) + " := " + p.second + " ";
        post_querry += "[execute].output" + std::to_string(in_num) + " ";
        in_num++;
    }
    command += post_querry + "\"";

    const char* command_char = command.c_str();
    FILE* pipe = popen(command_char, "r");
    if (!pipe) {
        std::cerr << "Ошибка при выполнении команды" << std::endl;
        return;
    }
    
    char buffer[128];
    std::string lastLine;
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        lastLine = buffer;
    }
    
    int status = pclose(pipe);
    if (status == -1) {
        std::cerr << "Ошибка при закрытии pipe" << std::endl;
    } else {
        std::cout << "Команда завершилась с кодом: " << status << std::endl;
    }
    
    outputs->setOutput(this->FBname+".OUT", lastLine);
}


// ./STInterpreter/INTERPRETER  --file="./STBlocks/sqrtST.ST" --program="execute" --pre-run-queries="[execute].input1:=3.14" --post-run-queries="[execute].output1"