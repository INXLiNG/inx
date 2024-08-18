add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE 
    inx/core/input.cpp
    inx/platform/opengl/opengl_shader.cpp
    inx/platform/opengl/opengl_texture.cpp
    inx/platform/opengl/opengl_buffer.cpp
    inx/platform/opengl/opengl_vertex_array.cpp
    inx/platform/opengl/opengl_render.cpp

    inx/renderer/buffers.cpp
    inx/renderer/camera.cpp
    inx/renderer/render_api.cpp
    inx/renderer/render2d.cpp

    inx/resources/shader.cpp
    inx/resources/texture.cpp

    app.h
    cubes.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC RES_PATH="${CMAKE_SOURCE_DIR}/res") # dev
# target_compile_definitions(${PROJECT_NAME} PUBLIC RES_PATH="./res") # release

target_link_libraries(${PROJECT_NAME} dep)