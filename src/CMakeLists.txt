set(srcs ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(LIBRARY_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../libs)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

set(YAML_INCLUDE_DIR ${LIBRARY_FOLDER}/yaml/include)
add_library(YAML_LIBRARY STATIC IMPORTED)

if (APPLE)
  set_target_properties(YAML_LIBRARY PROPERTIES IMPORTED_LOCATION
    ${LIBRARY_FOLDER}/yaml/lib/osx/libyaml-cpp.a
  )
elseif (UNIX)
  
endif()

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}  
  ${YAML_INCLUDE_DIR}
  .
#  common
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
  gfx/filters
  i18n
  lbx
  platform
  save
  ui
  ui/animations
  ui/parts
  ui/views
  ui/views/debug
)

add_subdirectory(common)
add_subdirectory(game)
add_subdirectory(gfx)
add_subdirectory(i18n)
add_subdirectory(platform)
add_subdirectory(save)
add_subdirectory(ui)

add_executable(openmom ${srcs})

target_link_libraries(openmom
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${LIBRARY_FOLDER}/yaml/lib/osx/libyaml-cpp.a
  COMMON_LIB
  GAME_LIB
  CITIES_LIB
  COMBAT_LIB
  MECHANICS_LIB
  PLAYERS_LIB
  SKILLS_LIB
  SPELLS_LIB
  UNITS_LIB
  WORLD_LIB
  GFX_LIB
  I18N_LIB
  PLATFORM_LIB
  SAVE_LIB
  UI_LIB
  UI_PARTS_LIB
  UI_ANIMATIONS_LIB
  VIEWS_LIB
  DEBUG_VIEWS_LIB
)