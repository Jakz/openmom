file(GLOB UI_SOURCE *.cpp)
add_library(UI_LIB ${UI_SOURCE})

add_subdirectory(parts)
add_subdirectory(views)
