#include "graph.h"


Graph::~Graph(){
    start.clear();
    all.clear();
    outputsAgregtor->clear();
}

void Graph::BFS(std::atomic_bool& isGraph){
    std::queue<IFB*> queue;
    for(const auto& block:start){
        queue.push(block);
    }

    while(!queue.empty() && isGraph){
        auto* block=queue.front();
        queue.pop();

        auto outputs = outputsAgregtor->getOutputs();

        auto inputs = block->getInputs();

        // for(const auto a:inputs){
        //     std::cout<<a.first<<" "<<a.second<<"\n";
        // }
        //std::cout<<"popo4ka"<<"\n";

        auto newInputs = inputs;

        std::map<std::string, std::string> connections=block->getConnections();

        // for(const auto& c:connections){
        //     std::cout<<c.first<<" "<<c.second<<"\n";
        // }

        // for(const auto& c:outputs){
        //     std::cout<<c.first<<" "<<c.second<<"\n";
        // }

        for(const auto& input:inputs){
            if(outputs[connections[input.first]]!=""){
                newInputs[input.first]=outputs[connections[input.first]];
            }
        }

        block->setInputs(newInputs);
        block->call(outputsAgregtor);
        auto next=block->getNext();

        for(const std::string& name:next){
            for(const auto& block:all){
                auto s = name.substr(0, name.size()-4);
                if(s==block->getName()){
                    
                    queue.push(block);
                }
            }
        }
    }
}
