set(APP_NAME "main_app")
set(SOURCE_DIR ".")
set(PRIVATE_HEADER_DIR ".")
set(PUBLIC_HEADER_DIR ".")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

add_executable(${APP_NAME} ${SOURCES})

target_include_directories(
  ${APP_NAME}
  PUBLIC ${PUBLIC_HEADER_DIR}
  PRIVATE ${PRIVATE_HEADER_DIR})

target_link_libraries(${APP_NAME} PRIVATE project_warnings pugixml)

include_directories(
    "../../build/_deps/pugixml-src/src"
)

set(SOURCE
    parser.cpp
    graph.cpp
    server.cpp
    )
 

add_library(App ${SOURCE})
   