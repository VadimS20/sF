add_subdirectory(modules)

set(SOURCE
    "./IFB.cpp"
    "./globalOutputsList.cpp"
    )

add_library(TypeLib ${SOURCE})