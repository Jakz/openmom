cmake_minimum_required(VERSION 3.0.2)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(APPLE)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(CMAKE_C_COMPILER "/usr/bin/clang")
elseif(UNIX)

elseif(WIN32)
#set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl2)
set(SDL2_LIBRARY C:\\SDL\\SDL2\\lib)
set(SDL2_INCLUDE_DIR C:\\SDL\\SDL2\\include)

set(SDL2_IMAGE_LIBRARY C:\\SDL\\SDL2_image\\lib)
set(SDL2_IMAGE_INCLUDE_DIR C:\\SDL\\SDL2_image\\include)
endif()



if(APPLE)
  set(BASE_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping --coverage -Wno-reorder -std=c++14 -stdlib=libc++ -fdiagnostics-show-option") # -Wall
elseif(UNIX)
  set(BASE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -Wno-inconsistent-missing-override -std=c++14 -stdlib=libstdc++ -fdiagnostics-show-option") # -Wall
elseif(WIN32)
  add_compile_options(/wd4068) # unknown pragma
  add_compile_options(/wd4244) # conversion from T to U, possible loss of data
  add_compile_options(/wd4267) # conversion from T to U, possible loss of data
  add_compile_options(/wd4996) # this function or variable may be unsafe
  add_compile_options(/wd4390) # empty controlled statement found
  add_compile_options(/wd4715) # not all control paths return a value
endif()


add_definitions(-DCMAKE_BUILD)
if (DEBUG_LEVEL)
  add_definitions(-DDEBUG=${DEBUG_LEVEL})
endif()

#set(BASE_FLAGS "${BASE_FLAGS} -fno-rtti -fno-exceptions")

if (WIN32)
set(DEBUG_FLAGS "")
else()
set(DEBUG_FLAGS "-O0 -g -fstandalone-debug")
endif()


set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS} -DDEBUG=1 ${BASE_FLAGS}" CACHE FILEPATH "cache")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3 -flto -DNDEBUG ${BASE_FLAGS}" CACHE FILEPATH "cache")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -DNDEBUG ${BASE_FLAGS}" CACHE FILEPATH "cache")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -flto -g ${BASE_FLAGS}" CACHE FILEPATH "cache")

set(CMAKE_C_FLAGS_DEBUG "${DEBUG_FLAGS} -DDEBUG=1" CACHE FILEPATH "cache")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-flto -O3 -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -flto -g" CACHE STRING "" FORCE)

message("Build Type is ${CMAKE_BUILD_TYPE}")
message("Base flags are ${BASE_FLAGS}")
message("Debug flags are ${CMAKE_CXX_FLAGS_DEBUG}")

#if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
#  message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
#endif()

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project("openmom" CXX C)

add_subdirectory(src)


