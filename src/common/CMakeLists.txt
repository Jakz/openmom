file(GLOB COMMON_SOURCE *.cpp)
add_library(COMMON_LIB ${COMMON_SOURCE} fmt/format.cc)