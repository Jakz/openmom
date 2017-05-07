file(GLOB VIEWS_SOURCE *.cpp)
add_library(VIEWS_LIB ${VIEWS_SOURCE})

add_subdirectory(debug)
