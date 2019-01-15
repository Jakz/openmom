set(srcs ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

#if (NOT WIN32)
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
#endif()

set(YAML_INCLUDE_DIR ${LIBRARY_FOLDER}/yaml/include)
add_library(YAML_LIBRARY STATIC IMPORTED)

if (APPLE)
  set_target_properties(YAML_LIBRARY PROPERTIES IMPORTED_LOCATION
    ${LIBRARY_FOLDER}/yaml/lib/osx/libyaml-cpp.a
  )
  set(YAML_LIBRARY_BINARY "${LIBRARY_FOLDER}/yaml/lib/osx/libyaml-cpp.a")
elseif(WIN32)
  #set_target_properties(YAML_LIBRARY PROPERTIES IMPORTED_LOCATION
  #  ${LIBRARY_FOLDER}/yaml/lib/win_x86/yaml-cpp.lib
  #)
  #set(YAML_LIBRARY_BINARY "${LIBRARY_FOLDER}/yaml/lib/win_x86/yaml-cpp.lib")

  set(YAML_LIBRARY_BINARY "${LIBRARY_FOLDER}/yaml/lib/win_x86/libyaml-cppmdd.lib")

  #add_library(SDL2_LIBRARY STATIC IMPORTED)
  #set_target_properties(SDL2_LIBRARY PROPERTIES IMPORTED_LOCATION
	#"${LIBRARY_FOLDER}/sdl2/win/lib/x86/SDL2.lib"
  #)
  set(SDL2_LIBRARY "${LIBRARY_FOLDER}/sdl2/win/lib/x86/SDL2.lib")
  set(SDL2_IMAGE_LIBRARIES "${LIBRARY_FOLDER}/sdl2/win/lib/x86/SDL2_image.lib")
elseif (UNIX)
  
  set_target_properties(YAML_LIBRARY PROPERTIES IMPORTED_LOCATION
    ${LIBRARY_FOLDER}/yaml/lib/linux/libyaml-cpp.a
  )
  set(YAML_LIBRARY_BINARY "${LIBRARY_FOLDER}/yaml/lib/linux/libyaml-cpp.a")
endif()

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}  
  ${YAML_INCLUDE_DIR}
  .
  common
  common/fmt
  game
  game/cities
  game/combat
  game/mechanics
  game/players
  game/skills
  game/spells
  game/units
  game/world
  gfx
  i18n
  lbx
  platform
  save
  ui
  ui/animations
  ui/parts
)

add_subdirectory(common)
add_subdirectory(game)
add_subdirectory(gfx)
add_subdirectory(i18n)
add_subdirectory(platform)
add_subdirectory(save)
add_subdirectory(ui)

add_executable(openmom ${srcs})
#add_executable(openmom-tests tests/tests.cpp)

set(LIBRARIES
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_LIBRARY_BINARY}
  GAME_LIB
  CITIES_LIB
  MECHANICS_LIB
  PLAYERS_LIB
  SKILLS_LIB
  SPELLS_LIB
  WORLD_LIB
  UI_LIB
  EXTRA_VIEWS_LIB
  DEBUG_VIEWS_LIB
  VIEWS_LIB
  COMMON_LIB
  COMBAT_LIB
  UNITS_LIB
  GFX_LIB
  I18N_LIB
  UI_PARTS_LIB
  UI_ANIMATIONS_LIB
  PLATFORM_LIB
  UI_LIB
  SAVE_LIB
  SPELLS_LIB
  SKILLS_LIB
  ${YAML_LIBRARY_BINARY}
)

target_link_libraries(openmom ${LIBRARIES})
#target_link_libraries(openmom-tests ${LIBRARIES})

#if(WIN32)
#  add_custom_command(TARGET openmom POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	"${LIBRARY_FOLDER}/sdl2/win/lib/x86/SDL2.dll"
#	${CMAKE_BINARY_DIR}
# )
#endif()
