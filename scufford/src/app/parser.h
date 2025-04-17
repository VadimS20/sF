#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <pugixml.hpp>
#include "../typeLib/IFB.h"
#include "../typeLib/globalOutputsList.h"
#include "../typeLib/modules/FBConsoleOut.h"
#include "../typeLib/modules/FBSumOfTwo.h"
#include "../typeLib/modules/BasicSTFB.h"


class Parser{
public:
    static std::pair<std::vector<IFB*>, GlobalOutputs*> parse(std::string pathToFile);
    static std::pair<std::vector<IFB*>, GlobalOutputs*> parseFboot(std::string pathToFile);
};
