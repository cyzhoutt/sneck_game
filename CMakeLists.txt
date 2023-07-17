cmake_minimum_required(VERSION 3.9)

project(Sneck C)
set(CMAKE_C_STANDARD 11)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SBSProject_SOURCE_DIR})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(tests COMMAND make -C ${sneck_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR} snake)

add_custom_target(unit-tests COMMAND make -C ${sneck_SOURCE_DIR} run-unit-tests)

#add_executable(unit-tests-2 snake_utils.c unit_tests.c asserts.c snake_utils.h asserts.h state.c state.h)
add_executable(unit-tests-2 snake_utils.c unit_tests.c asserts.c snake_utils.h asserts.h)

add_executable(integration-test snake_utils.c unit_tests.c asserts.c snake_utils.h asserts.h)


add_executable(snake snake_utils.c asserts.c snake_utils.h asserts.h state.c state.h snake.c)
#cmake_minimum_required(VERSION 3.9)
#project(Snek C)
#
#set(CMAKE_C_STANDARD 11)
#
#add_executable(Snek snake.c snake_utils.c snake_utils.h state.c state.h)