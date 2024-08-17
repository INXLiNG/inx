set(SDL_TEST OFF)

add_subdirectory(SDL)
add_subdirectory(glad)
add_subdirectory(stb)

set(GLM_BUILD_LIBRARY OFF)
add_subdirectory(glm)

# i hate cmake
# i am not moving this out of this file
# i do not know how
# sue me.

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_library(ImGui STATIC)

target_sources(ImGui PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
)

target_include_directories(ImGui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(ImGui PUBLIC SDL3-shared)

add_library(dep INTERFACE)
target_link_libraries(dep INTERFACE SDL3::SDL3 glad stb glm::glm ImGui)