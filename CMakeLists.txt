cmake_minimum_required(VERSION 3.20)
project(tracer)

include(FindPkgConfig)
find_package(GLEW REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result -Wno-deprecated-declarations")

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O0 -g ${WARNING_FLAGS} -fdeclspec -fsanitize=address -march=native -DGL_SILENCE_DEPRECATION")

add_executable(tracer
        main.cpp
        graphics/gl/program.cpp
        graphics/gl/program.hpp
        graphics/gl/shader.cpp
        graphics/gl/shader.hpp
        graphics/gl/gl_buffer.cpp
        graphics/gl/gl_buffer.hpp
        graphics/gl/vertex_fragment_program.cpp
        graphics/gl/vertex_fragment_program.hpp
        graphics/window.cpp
        graphics/window.hpp
        graphics/gl/vertex_array_object_factory.cpp
        graphics/gl/vertex_array_object_factory.hpp
        graphics/programs/triangle_program.cpp
        graphics/programs/triangle_program.hpp
        graphics/programs/path_tracer_program.cpp
        graphics/programs/path_tracer_program.hpp
        graphics/error_handling.cpp
        graphics/error_handling.hpp
        graphics/gl/uniform.cpp
        graphics/gl/uniform.hpp
        graphics/gl/gl_texture_buffer.cpp
        graphics/gl/gl_texture_buffer.hpp
        graphics/programs/camera_uniform_controller/camera_uniform_controller.cpp
        graphics/programs/camera_uniform_controller/camera_uniform_controller.hpp
        graphics/drawing_context.cpp
        graphics/drawing_context.hpp
        utils/vec3.hpp
        utils/matrix4.hpp
        utils/vec3.cpp
        controls/user_controller.hpp
        controls/user_controller.cpp
        ui/ui_view.cpp
        ui/ui_view.hpp
        ui/ui_slider.cpp
        ui/ui_slider.hpp
        ui/ui_button.cpp
        ui/ui_button.hpp
        ui/ui_text.cpp
        ui/ui_text.hpp
        events/mouse_events.cpp
        events/mouse_events.hpp
        utils/vec2.cpp
        utils/vec2.hpp
        utils/matrix3.hpp
        utils/vec4.hpp utils/vec4.cpp graphics/camera.cpp graphics/camera.hpp scene/scene.cpp scene/scene.hpp scene/hittables/hittable_list.cpp scene/hittables/hittable_list.hpp scene/hittables/hittable.cpp scene/hittables/hittable.hpp scene/hittables/hittable_sphere.cpp scene/hittables/hittable_sphere.hpp scene/hittables/hittable_triangle.cpp scene/hittables/hittable_triangle.hpp)

include_directories(tracerc ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SFML_INCLUDE_DIRS})
target_link_libraries(tracer GLEW::GLEW OpenGL sfml-graphics)