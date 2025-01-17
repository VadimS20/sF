#include "parser.h"


std::pair<std::vector<IFB*>, GlobalOutputs*> Parser::parse(std::string pathToFile,std::string& pathToEsstee){
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(pathToFile.c_str());

    if (!result) {
        std::cerr << "XML parsed with errors, error description: "
            << result.description()
            << "\n";
    }

    auto root=doc.child("FB_Blocks");
    std::vector<IFB*> FBs;

    std::map<std::string, std::string> outputs;
    
    for (pugi::xml_node fbNode = root.child("FunctionBlock"); fbNode; fbNode = fbNode.next_sibling("FunctionBlock")) {
        std::string name = fbNode.child("Name").text().as_string();
        std::string type = fbNode.child("Type").text().as_string();
        std::string description = fbNode.child("Description").text().as_string();
        auto var = fbNode.child("Interface");

        std::map<std::string, std::string> inputs;
        for (pugi::xml_node inputNode = var.child("InputVariables").child("Variable"); inputNode; inputNode = inputNode.next_sibling("Variable")) {
            inputs[inputNode.child("Name").text().as_string()] = inputNode.child("Value").text().as_string();
        }
        
        for (pugi::xml_node outputNode = var.child("OutputVariables").child("Variable"); outputNode; outputNode = outputNode.next_sibling("Variable")) {
            outputs[outputNode.child("Name").text().as_string()] = "";
        }

        std::map<std::string, std::string> connections;
        std::vector<std::string> next;

        for (pugi::xml_node connNode = var.child("Connections").child("Connection"); connNode; connNode = connNode.next_sibling("Connection")) {
            std::string connTo = connNode.child("Target").text().as_string();
            
            if (connTo.find("REQ")!=std::string::npos) {
                next.push_back(connTo);
            } else { 
                connections[connTo] = connNode.child("Source").text().as_string();
            }
        }

        if (type == "FBSumOfTwo") {
            FBs.push_back(new FBSumOfTwo(inputs, connections, next, name));
        } else if (type == "FBConsoleOut") {
            FBs.push_back(new FBConsoleOut(inputs, connections, next, name));
        } else {
            auto tempBlock=new FBConsoleOut(inputs,connections,next,name);
            tempBlock->setPathToESSTEE(pathToEsstee);
            FBs.push_back(tempBlock);
        }
        
    }
    GlobalOutputs* Output = GlobalOutputs::getInstance(outputs);
    remove("received_file.xml");
    return std::make_pair(FBs, Output);
}



std::pair<std::vector<IFB*>, GlobalOutputs*> Parser::parseFboot(std::string pathToFile,std::string& pathToEsstee){
    std::stringstream xmlStream;

    std::string line;
 
    std::ifstream in(pathToFile); 
    if (in.is_open())
    {
        while (std::getline(in, line, ';')) {
            if (line.find("<Request") != std::string::npos) {
                xmlStream << line << "\n";
            }
        }
    }
    in.close(); 



    std::string xmlContent=xmlStream.str();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xmlContent.c_str());

    if (!result) {
        std::cerr << "Failed to parse XML: " << result.description() << std::endl;
        
    }

    std::vector<IFB*> FBs;
    std::map<std::string,IFB*> FBsMap;

    std::map<std::string,std::string> inputs;
    std::map<std::string,std::string> outputs;
    std::map<std::string,std::string> conns;
    std::string start;

    std::string name="";
    std::string type="";

    for (pugi::xml_node request = doc.child("Request"); request; request = request.next_sibling("Request")) {
        std::string id = request.attribute("ID").as_string();
        std::string action = request.attribute("Action").as_string();


        if(action=="CREATE"){
            if(name==""){
                pugi::xml_node fb = request.child("FB");
                if (fb) {
                    name = fb.attribute("Name").as_string();
                    type = fb.attribute("Type").as_string();
                    

                }
                pugi::xml_node connection=request.child("Connection");
                if(connection){
                    name="";
                    type="";
                    std::string source = connection.attribute("Source").as_string();
                    std::string destination = connection.attribute("Destination").as_string();
                    if(source.find("START")!=std::string::npos){
                        start=destination.substr(0,destination.length()-4);
                    }else{
                        inputs[destination]="";
                        conns[destination]=source;
                        outputs[source]="";
                    }
                    if(source.find(".CNF")!=std::string::npos){
                        FBsMap[source.substr(0,source.length()-4)]->addNext(destination);
                    }

                }


            }else{
                pugi::xml_node connection=request.child("Connection");
                
                std::vector<std::string> next;

                if (type == "FBSumOfTwo") {            
                    FBsMap[name]=new FBSumOfTwo(inputs, conns, next, name);        
                    FBs.push_back(FBsMap[name]);
                } else if (type == "FBConsoleOut") {
                    FBsMap[name]=new FBConsoleOut(inputs, conns, next, name);
                    FBs.push_back(FBsMap[name]);
                } else {
                    FBsMap[name]=new FBSt(inputs,conns,next,name);
                    FBsMap[name]->setPathToESSTEE(pathToEsstee);
                    FBs.push_back(FBsMap[name]);
                }    

                inputs.clear();
                pugi::xml_node fb = request.child("FB");
                if (fb) {
                    name = fb.attribute("Name").as_string();
                    type = fb.attribute("Type").as_string();
                }
                if(connection){
                    name="";
                    type="";
                    std::string source = connection.attribute("Source").as_string();
                    std::string destination = connection.attribute("Destination").as_string();
                    if(source.find(".")==std::string::npos){
                        inputs[destination]=source;
                    }else{
                        inputs[destination]="";
                        conns[destination]=source;
                        outputs[source]="";
                    }
                    if(source.find(".CNF")!=std::string::npos){
                        FBsMap[source.substr(0,source.length()-4)]->addNext(destination);
                        std::cout<<source.substr(0,source.length()-4)<<"\n";
                    }

                }
            }
            
        }else{
            pugi::xml_node connection = request.child("Connection");
            if (connection) {
                std::string source = connection.attribute("Source").as_string();
                std::string destination = connection.attribute("Destination").as_string();
                if(source.find(".")==std::string::npos){
                        inputs[destination]=source;
                    }else{
                        inputs[destination]="";
                        conns[destination]=source;
                        outputs[source]="";
                    }
                
            }  
        }       
    }
    for(const auto FB:FBs){
        FB->setConnections(conns);
    }

    int startId=0;

    for(int i=0;i<FBs.size();i++){
        if(FBs[i]->getName()==start){
            startId=i;
            break;
        }
    }

    iter_swap(FBs.begin(),FBs.begin()+startId);

    GlobalOutputs* Output = GlobalOutputs::getInstance(outputs);

    remove("received_file.fboot");

    return std::make_pair(FBs, Output);    
}