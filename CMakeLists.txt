cmake_minimum_required(VERSION 3.20)
project(tracer)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(SDL2 REQUIRED)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result -fsanitize=address -Wno-deprecated-declarations")

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O0 -g ${WARNING_FLAGS} -fdeclspec -march=native -DGL_SILENCE_DEPRECATION")

include_directories(tracer ${SDL2_INCLUDE_DIRS})

add_executable(tracer
        main.cpp
        graphics/program.cpp
        graphics/program.hpp
        graphics/shader.cpp
        graphics/shader.hpp
        graphics/gl_buffer.cpp
        graphics/gl_buffer.hpp
        graphics/vertex_fragment_program.cpp
        graphics/vertex_fragment_program.hpp
        graphics/window.cpp
        graphics/window.hpp
        graphics/vertex_array_object_factory.cpp
        graphics/vertex_array_object_factory.hpp
        graphics/programs/triangle_program.cpp
        graphics/programs/triangle_program.hpp
        graphics/error_handling.cpp
        graphics/error_handling.hpp)

target_link_libraries(tracer ${SDL2_LIBRARIES} ${OPENGL_gl_LIBRARY})