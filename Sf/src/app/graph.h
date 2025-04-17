#include <queue>
#include <atomic>
#include "iostream"
#include "../typeLib/IFB.h"
#include "../typeLib/globalOutputsList.h"

class Graph{
private:
    std::vector<IFB*> all;
    std::vector<IFB*> start;
    GlobalOutputs* outputsAgregtor;

public:
    IFB search(std::string nameFB);
    void BFS(std::atomic_bool& isGraph);
    Graph(std::vector<IFB*> start, std::vector<IFB*> all, GlobalOutputs* outputsAgregtor) :
    start(start), all(all), outputsAgregtor(outputsAgregtor) {};
    ~Graph();
};