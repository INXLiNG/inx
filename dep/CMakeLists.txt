set(SDL_TEST OFF)

add_subdirectory(SDL)
add_subdirectory(glad)
add_subdirectory(stb)

set(GLM_BUILD_LIBRARY OFF)
add_subdirectory(glm)

add_library(dep INTERFACE)
target_link_libraries(dep INTERFACE SDL3::SDL3 glad stb glm::glm)