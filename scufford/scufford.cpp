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

#include "./src/app/request_receiver.hpp"
#include "./src/app/server.hpp"
#include "./src/app/graph.h"
#include "./src/app/parser.h"
#include "./src/typeLib/IFB.h"
#include "./src/typeLib/modules/FBSumOfTwo.h"
#include "./src/typeLib/modules/FBConsoleOut.h"
#include "./src/typeLib/modules/FBSt.h"


void graphExecution(std::string xmlFile, std::atomic_bool& isGraph, std::string& pathToEsstee){
    std::pair<std::vector<IFB*>, GlobalOutputs*> pair;

    std::cout<<"graph"<<"\n";
    if(xmlFile.find(".fboot")!=std::string::npos){
        pair=Parser::parseFboot(xmlFile,pathToEsstee);
    }else{
        pair=Parser::parse(xmlFile,pathToEsstee);
    }    
    auto all=pair.first;
    auto agregtor=pair.second;

    std::vector<IFB*> start = {};
    start.push_back(all[0]);
    auto graph=new Graph(start,all,agregtor);
    graph->BFS(isGraph);
    graph->~Graph();
}

void runApp(int &port, std::string &pathToFile, std::string &pathToEsstee){
    std::thread appThread;
    std::atomic_bool isGraph(true);
    if (pathToFile != ""){
        appThread = std::thread(graphExecution, pathToFile, std::ref(isGraph), pathToEsstee);
    }

    while (1)
    {
        std::string err;

        Server server(port,err);
        //
        
        std::string erorr_message;

        server.WaitForConnection(erorr_message);
        std::cout << erorr_message << std::endl;

        ErrorCode error;

        // Открываем файл для записи (режим добавления в конец файла)
        std::ofstream outfile("recieved_data.fboot", std::ios::app);

        while (true) {
            auto received_request = ReceiveRequest(server, error).value();
            if (received_request.xml_string.length() == 1) {
                std::cout << "CONNECTION CLOSED" << '\n';
                break;
            }

            // Записываем полученные данные в файл
            outfile << received_request.xml_string << std::endl;

            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_string(received_request.xml_string.c_str());

            pugi::xml_node root = doc.document_element();

            if (std::string(root.attribute("Action").as_string()) == "CREATE") {
                pugi::xml_node child = root.first_child();
                std::cout << std::string(child.name()) << std::endl;
            }

            pugi::xml_attribute id_attribute = root.attribute("ID");
            uint response_index = id_attribute.as_uint();

            std::string respose = "<Response ID=\"" + std::to_string(response_index) + "\" />";
            server.Send<char>('P');
            server.Send<uint16_t>(respose.size());
            server.SendString(respose);
            std::cout << "Sent response: " << respose << '\n';

            std::cout << "\n\n";

            std::cout << "Received request: " << received_request.xml_string << '\n';
        }


        outfile.close();

        
        //std::thread serv(server.Start());
        server.Start();

        if (std::filesystem::exists("./received_file.xml")) {
            std::cerr<<"File received"<<std::endl;
            if (appThread.joinable()) {
                isGraph = false;
                appThread.detach();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            isGraph = true;
            appThread = std::thread(graphExecution, "received_file.xml", std::ref(isGraph),pathToEsstee);
        }
        if (std::filesystem::exists("./received_file.fboot")) {
            std::cerr<<"File received"<<std::endl;
            if (appThread.joinable()) {
                isGraph = false;
                appThread.detach();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            isGraph = true;
            appThread = std::thread(graphExecution, "received_file.fboot", std::ref(isGraph),pathToEsstee);
        }
        //serv.join();
    }
}

int main(int argc, char *argv[]) {
    std::string pathToFile;
    int port;
    remove("received_file.xml");
    argparse::ArgumentParser program("scufford");
    
    program.add_argument("-f", "--file")
        .default_value("")
        .help("input .xml/.fboot filename");

    program.add_argument("-esstee")
        .default_value("")
        .help("input esstee path");    

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

    std::string pathToEsstee;
    pathToEsstee = program.get("-esstee");
    pathToFile = program.get("-f");
    port = program.get<int>("-p");
    
    runApp(port, pathToFile,pathToEsstee);
    return 0;
}