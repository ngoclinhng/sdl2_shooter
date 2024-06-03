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
  utils.c
  game_context.c
  events.c
  textures.c
  entity.c
  game_world.c
  frame_rate.c
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

if (BUILD_TESTING)
  macro(SHOOTER_TEST TESTNAME)
    add_executable(${TESTNAME}_test ${TESTNAME}_test.c ${ARGN})

    target_include_directories(${TESTNAME}_test
      PRIVATE
      ${SHOOTER_PRIVATE_DEPENDENCIES_INCLUDE_DIRS}
      ${Shooter_SOURCE_DIR}/src      
      ${CMOCKA_INCLUDE_DIRS})

    target_link_libraries(${TESTNAME}_test
      PRIVATE
      ${SHOOTER_PRIVATE_DEPENDENCIES}
      ${CMOCKA_LIBRARIES})

    set_target_properties(${TESTNAME}_test
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

    add_test(NAME ${TESTNAME}_test
      COMMAND ${CMAKE_BINARY_DIR}/tests/${TESTNAME}_test)
  endmacro()

  shooter_test(entity entity.c textures.c)
  shooter_test(game_context game_context.c)
endif()
