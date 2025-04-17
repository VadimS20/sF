#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <filesystem>
#include <thread>
#include <stop_token>

#include <argparse/argparse.hpp>

#include "./src/app/server.h"
#include "./src/app/graph.h"
#include "./src/app/parser.h"
#include "./src/typeLib/IFB.h"
#include "./src/typeLib/modules/FBSumOfTwo.h"
#include "./src/typeLib/modules/FBConsoleOut.h"


void graphExecution(std::string xmlFile, std::atomic_bool& isGraph){
    std::pair<std::vector<IFB*>, GlobalOutputs*> pair;

    std::cout<<"graph"<<"\n";
    if(xmlFile.find(".fboot")!=std::string::npos){
        pair=Parser::parseFboot(xmlFile);
    }else{
        pair=Parser::parse(xmlFile);
    }    
    auto all=pair.first;
    auto agregtor=pair.second;

    std::vector<IFB*> start = {};
    start.push_back(all[0]);
    auto graph=new Graph(start,all,agregtor);
    graph->BFS(isGraph);
    graph->~Graph();
}

void runApp(int &port, std::string &pathToFile){
    std::thread appThread;
    std::atomic_bool isGraph(true);
    if (pathToFile != ""){
        appThread = std::thread(graphExecution, pathToFile, std::ref(isGraph));
    }

    while (1)
    {
        std::thread serv(Server::server, port);
        
        if (std::filesystem::exists("./received_file.xml")) {
            std::cerr<<"File received"<<std::endl;
            if (appThread.joinable()) {
                isGraph = false;
                appThread.detach();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            isGraph = true;
            appThread = std::thread(graphExecution, "received_file.xml", std::ref(isGraph));
        }
        if (std::filesystem::exists("./received_file.fboot")) {
            std::cerr<<"File received"<<std::endl;
            if (appThread.joinable()) {
                isGraph = false;
                appThread.detach();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            isGraph = true;
            appThread = std::thread(graphExecution, "received_file.fboot", std::ref(isGraph));
        }
        serv.join();
    }
}

int main(int argc, char *argv[]) {
    std::string pathToFile;
    int port;

    argparse::ArgumentParser program("scufford");
    
    program.add_argument("-f", "--file")
        .default_value("")
        .help("input .xml filename");

    program.add_argument("-p", "--port")
        .default_value(61499)
        .help("specify port to connect")
        .scan<'i', int>();

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    pathToFile = program.get("-f");
    port = program.get<int>("-p");
    
    runApp(port, pathToFile);
    return 0;
}