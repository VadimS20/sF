cmake_minimum_required(VERSION 3.16)
project(SCUFFORD VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall")

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

FetchContent_Declare(esstee
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG        v1.14
)
FetchContent_MakeAvailable(pugixml)


add_executable(SCUFFORD scufford.cpp)

target_link_libraries(SCUFFORD pugixml::pugixml)
target_link_libraries(SCUFFORD argparse::argparse)
target_link_libraries(SCUFFORD App TypeLib Modules)