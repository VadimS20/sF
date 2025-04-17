#include "BasicSTFB.h"

void BasicSTFB::execute(GlobalOutputs* outputs){
    std::string command = "./StInterpreter/INTERPRETER --file \"./StBlocks/" + this->FBname + ".st\" --program=\"testprgm\" --pre-run-queries=\"";
    int in_num = 1;
    std::string post_querry = "\" --post-run-queries=\"";
    for(const auto p : this->inputs){
        command += "[execute].input" + std::to_string(in_num) + " := " + p.second + ";";
        post_querry += "[execute].output" + std::to_string(in_num) + " := " + p.second + ";";
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
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;
    }
    
    int status = pclose(pipe);
    if (status == -1) {
        std::cerr << "Ошибка при закрытии pipe" << std::endl;
    } else {
        std::cout << "Команда завершилась с кодом: " << status << std::endl;
    }

    std::string buffer_str(buffer);
    outputs->setOutput(this->FBname+".OUT", buffer_str);
}