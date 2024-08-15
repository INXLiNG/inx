add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE 
    main.cpp 
    platform/opengl/opengl_shader.cpp
    platform/opengl/opengl_texture.cpp
    resources/shader.cpp
    resources/texture.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE RES_PATH="${CMAKE_SOURCE_DIR}/res") # dev
# target_compile_definitions(${PROJECT_NAME} PUBLIC RES_PATH="./res") # release

target_link_libraries(${PROJECT_NAME} PRIVATE dep)