cmake_minimum_required(VERSION 3.18)
project(Pong)
set(SOURCES main.c game.h game.c object.h object.c util.h util.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -Wpedantic")
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
add_executable(Pong ${SOURCES})
target_link_libraries(Pong ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} "m")
add_custom_command(
    TARGET Pong
    POST_BUILD
    COMMAND "cp" ARGS -rp ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR})
install(TARGETS Pong RUNTIME DESTINATION .)
install(DIRECTORY assets DESTINATION .)
include (CPack)