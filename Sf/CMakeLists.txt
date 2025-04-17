cmake_minimum_required(VERSION 3.16)
project(Sf VERSION 0.1.0)

add_subdirectory(src)
include_directories(
    "./",
    "./src",
    "./src/typeLib",
    "./src/typeLib/modules",
    "./src/app",
    "./build/_deps/argparse-src/include"
)

include(FetchContent)

FetchContent_Declare(pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG        v1.14
)
FetchContent_MakeAvailable(pugixml)

FetchContent_Declare(argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  GIT_TAG        v3.1
)
FetchContent_MakeAvailable(argparse)

add_executable(Sf Sf.cpp)

target_link_libraries(Sf pugixml::pugixml)
target_link_libraries(Sf argparse::argparse)
target_link_libraries(Sf App TypeLib Modules)