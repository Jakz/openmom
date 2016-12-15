set(srcs
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}  
  .
  common
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
  ui/parts
  ui/views
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
  VIEWS_LIB
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
)