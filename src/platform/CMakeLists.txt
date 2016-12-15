if (APPLE)
  set (PLATFORM_SOURCE osx.mm)
  
  set_source_files_properties(
    osx.mm
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )
else()
  
endif()

add_library(PLATFORM_LIB ${PLATFORM_SOURCE})
