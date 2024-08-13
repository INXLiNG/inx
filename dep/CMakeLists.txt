set(SDL_TEST OFF)

add_subdirectory(SDL)
add_subdirectory(glad)
add_subdirectory(stb)

add_library(dep INTERFACE)
target_link_libraries(dep INTERFACE SDL3::SDL3 glad stb)