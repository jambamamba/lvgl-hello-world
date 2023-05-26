cmake_minimum_required(VERSION 3.10)
set (project "hello-world")
project(${project})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SCREEN_HOR_RES $ENV{SCREEN_HOR_RES})
set(SCREEN_VER_RES $ENV{SCREEN_VER_RES})
add_compile_definitions(SCREEN_HOR_RES=${SCREEN_HOR_RES})
add_compile_definitions(SCREEN_VER_RES=${SCREEN_VER_RES})

if(USE_WAYLAND)
    add_compile_definitions(USE_WAYLAND=1)
else()
    add_compile_definitions(USE_SDL=1)
endif()

add_subdirectory(lvgl-libraries)
set (sources
   main.cpp
)

set (inc_dirs
    lvgl-libraries
    rendering-engine
)
set(libs
    lvgl
)
set(link_dirs
)

if(USE_WAYLAND)
    list(APPEND sources rendering-engine/rendering_engine_wayland.c)
else()
    list(APPEND sources rendering-engine/rendering_engine_sdl.c)
endif()

add_executable("${project}" ${sources})
target_include_directories("${project}" PRIVATE ${inc_dirs})
target_link_directories("${project}" PRIVATE ${link_dirs})
target_link_libraries("${project}" PRIVATE ${libs})
