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

add_executable(openmom ${srcs})

target_link_libraries(openmom
  commonlib
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
)