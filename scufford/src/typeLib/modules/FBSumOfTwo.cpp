#include "FBSumOfTwo.h"

void FBSumOfTwo::execute(GlobalOutputs* outputs){
    int sum=0;

    for(const auto p : this->inputs){
        size_t hash_pos = p.second.find('#');
        if (hash_pos != std::string::npos) {
            // Выделяем подстроку после '#'
            std::string num_str = p.second.substr(hash_pos + 1);
            
            // Преобразуем в число
            int num = std::stoi(num_str);
            sum += num;
        }
        
    }
    outputs->setOutput(this->FBname+".OUT",std::to_string(sum));
}