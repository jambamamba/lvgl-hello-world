cmake_minimum_required(VERSION 3.10)
set (project "hello-world")
project(${project})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
