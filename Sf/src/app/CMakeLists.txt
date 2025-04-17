include_directories(
    "../../build/_deps/pugixml-src/src"
)

set(SOURCE
    parser.cpp
    graph.cpp
    server.cpp
    )

add_library(App ${SOURCE})