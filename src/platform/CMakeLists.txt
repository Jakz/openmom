if (APPLE)
  set (PLATFORM_SOURCE macos/osx.mm)
  
  set_source_files_properties(
    macos/osx.mm
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )
elseif(UNIX)
  set (PLATFORM_SOURCE unix.cpp)
elseif(WIN32)
  set (PLATFORM_SOURCE windows/windows.cpp)  
endif()

set (PLATFORM_SOURCE ${PLATFORM_SOURCE} Path.cpp)

add_library(PLATFORM_LIB ${PLATFORM_SOURCE})
