cmake_minimum_required(VERSION 3.16)
project(tracer)

include(FindPkgConfig)
find_package(OpenGL REQUIRED)
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
        graphics/gl/vertex_array_object_factory.cpp
        graphics/gl/vertex_array_object_factory.hpp
        graphics/programs/texture_program.cpp
        graphics/programs/texture_program.hpp
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
        events/mouse_events.cpp
        events/mouse_events.hpp
        utils/vec2.cpp
        utils/vec2.hpp
        utils/matrix3.hpp
        utils/vec4.hpp
        utils/vec4.cpp
        graphics/camera.cpp
        graphics/camera.hpp
        scene/scene.cpp
        scene/scene.hpp
        scene/hittables/hittable_list.cpp
        scene/hittables/hittable_list.hpp
        scene/hittables/hittable.cpp
        scene/hittables/hittable.hpp
        scene/hittables/hittable_sphere.cpp
        scene/hittables/hittable_sphere.hpp
        scene/hittables/hittable_triangle.cpp
        scene/hittables/hittable_triangle.hpp
        scene/scene_buffer.cpp
        scene/scene_buffer.hpp
        scene/materials/material.cpp
        scene/materials/material.hpp
        scene/scene_buffer_serializable.cpp
        scene/scene_buffer_serializable.hpp
        scene/materials/material_metal.cpp
        scene/materials/material_metal.hpp
        scene/scene_renderer.cpp
        scene/scene_renderer.hpp
        scene/materials/material_lambertian.cpp
        scene/materials/material_lambertian.hpp
        scene/materials/material_lambertian_light.cpp
        scene/materials/material_lambertian_light.hpp
        scene/build_cube.cpp
        scene/build_cube.hpp
        examples/cornell_box/cornell_box_example_view.cpp
        examples/cornell_box/cornell_box_example_view.hpp
        scene/build_rect.cpp
        scene/build_rect.hpp
        graphics/gl/gl_framebuffer.cpp
        graphics/gl/gl_framebuffer.hpp
        graphics/gl/gl_texture.cpp
        graphics/gl/gl_texture.hpp
        graphics/gl/gl_enums.cpp
        graphics/gl/gl_enums.hpp
        graphics/programs/accumulator_program.cpp
        graphics/programs/accumulator_program.hpp
        graphics/programs/present_program.cpp
        graphics/programs/present_program.hpp
        scene/hittables/bvh/bvh_node.cpp
        scene/hittables/bvh/bvh_node.hpp
        scene/aabb.cpp
        scene/aabb.hpp
        obj/tiny_obj_loader.h
        scene/scene_drawer.cpp
        scene/scene_drawer.hpp
        utils/frame_timer.cpp
        utils/frame_timer.hpp
        scene/build_model.hpp
        scene/build_model.cpp
        graphics/programs/bounded_program.cpp
        graphics/programs/bounded_program.hpp
        scene/materials/material_dielectric.cpp
        scene/materials/material_dielectric.hpp
        examples/cornell_box_sphere/cornell_box_sphere_example_view.cpp
        examples/cornell_box_sphere/cornell_box_sphere_example_view.hpp
        examples/tank/tank_example_view.cpp
        examples/tank/tank_example_view.hpp
        examples/example_view.cpp
        examples/example_view.hpp
        examples/random_spheres/random_spheres_example_view.cpp
        examples/random_spheres/random_spheres_example_view.hpp
        scene/buffer_chunk.cpp
        scene/buffer_chunk.hpp
        scene/hittables/bvh/bvh_tree.cpp
        scene/hittables/bvh/bvh_tree.hpp
        examples/playground/playground_view.cpp
        examples/playground/playground_view.hpp)

include_directories(tracerc ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SFML_INCLUDE_DIRS})
target_link_libraries(tracer GLEW::GLEW OpenGL::GL sfml-graphics)

add_custom_command(TARGET tracer PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

string(LENGTH ${CMAKE_SOURCE_DIR} CMAKE_SOURCE_DIR_LENGTH)

file(GLOB_RECURSE all_shaders "${CMAKE_SOURCE_DIR}/shaders/**/*.shader")
file(GLOB_RECURSE main_shaders "${CMAKE_SOURCE_DIR}/shaders/**/vertex.shader" "${CMAKE_SOURCE_DIR}/shaders/**/fragment.shader")

foreach(file ${main_shaders})
    string(SUBSTRING ${file} ${CMAKE_SOURCE_DIR_LENGTH} -1 file_relative)
    string(REGEX REPLACE "/[^/]*$" "" file_no_ext ${file_relative})
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources${file_no_ext})

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/resources${file_relative}
        COMMAND gcc -E -P -x c ${CMAKE_SOURCE_DIR}${file_relative} -o ${CMAKE_BINARY_DIR}/resources${file_relative}
        DEPENDS ${all_shaders}
    )

    list(INSERT output_shaders 0 ${CMAKE_BINARY_DIR}/resources${file_relative})

    #    message("Compiling shader ${CMAKE_SOURCE_DIR}${file} to ${CMAKE_BINARY_DIR}/resources${file}")
endforeach()

add_custom_target(shaders ALL DEPENDS ${output_shaders})