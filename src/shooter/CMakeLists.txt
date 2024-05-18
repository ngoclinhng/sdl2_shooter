unset(SHOOTER_PRIVATE_DEPENDENCIES)
unset(SHOOTER_PRIVATE_DEPENDENCIES_INCLUDE_DIRS)

if (SDL2_FOUND)
  list(APPEND SHOOTER_PRIVATE_DEPENDENCIES ${SDL2_LIBRARIES})
  list(APPEND SHOOTER_PRIVATE_DEPENDENCIES_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})  
endif()

if (SDL2_IMAGE_FOUND)
  list(APPEND SHOOTER_PRIVATE_DEPENDENCIES ${SDL2_IMAGE_LIBRARIES})
  list(APPEND SHOOTER_PRIVATE_DEPENDENCIES_INCLUDE_DIRS
    ${SDL2_IMAGE_INCLUDE_DIRS})
endif()

set(SHOOTER_SRCS
  init.c
  input.c
  draw.c
  main.c)

add_executable(shooter ${SHOOTER_SRCS})

target_include_directories(shooter PRIVATE
  ${Shooter_SOURCE_DIR}/src
  ${SHOOTER_PRIVATE_DEPENDENCIES_INCLUDE_DIRS})

target_link_libraries(shooter PRIVATE
  ${SHOOTER_PRIVATE_DEPENDENCIES})

set(GFX_SOURCE_DIR "${Shooter_SOURCE_DIR}/gfx")
set(GFX_OUTPUT_DIR "${CMAKE_BINARY_DIR}/gfx")

add_custom_target(copy_gfx ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GFX_SOURCE_DIR} ${GFX_OUTPUT_DIR}
  COMMENT "Copying gfx directory to build directory")

add_dependencies(shooter copy_gfx)
