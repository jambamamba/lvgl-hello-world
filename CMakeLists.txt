cmake_minimum_required(VERSION 3.10)
set (project "hello-world")
project(${project})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(USE_WAYLAND=1)
add_subdirectory(lvgl-libraries)
set (sources
   main.c
)

set (inc_dirs
    lvgl-libraries/lvgl/
)
set(libs
    lvgl
)
set(link_dirs
)

add_executable("${project}" ${sources})
target_include_directories("${project}" PRIVATE ${inc_dirs})
target_link_directories("${project}" PRIVATE ${link_dirs})
target_link_libraries("${project}" PRIVATE ${libs})
