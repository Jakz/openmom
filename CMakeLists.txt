cmake_minimum_required(VERSION 3.0.2)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(APPLE)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(CMAKE_C_COMPILER "/usr/bin/clang")
elseif(UNIX)

endif()



if(APPLE)
  set(BASE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -std=c++11 -stdlib=libc++ -fdiagnostics-show-option") # -Wall
elseif(UNIX)
  set(BASE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -Wno-inconsistent-missing-override -std=c++11 -stdlib=libstdc++ -fdiagnostics-show-option") # -Wall
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fstandalone-debug -fno-rtti -fno-exceptions -DDEBUG=1 ${BASE_FLAGS}" CACHE FILEPATH "cache")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3 -flto -DNDEBUG -fno-rtti -fno-exceptions ${BASE_FLAGS}" CACHE FILEPATH "cache")
set(CMAKE_CXX_FLAGS_RELEASE "-flto -O3 -DNDEBUG -fno-rtti -fno-exceptions ${BASE_FLAGS}" CACHE FILEPATH "cache")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -flto -g -fno-rtti -fno-exceptions ${BASE_FLAGS}" CACHE FILEPATH "cache")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -fstandalone-debug -DDEBUG=1" CACHE FILEPATH "cache")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-flto -O3 -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -flto -g" CACHE STRING "" FORCE)

message("Build Type is ${CMAKE_BUILD_TYPE}")
message("C flags are ${CMAKE_C_FLAGS}")
message("CXX flags are ${CMAKE_CXX_FLAGS}")

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project("openmom" CXX C)

add_subdirectory(src)


