set(SDL_TEST OFF)
add_subdirectory(SDL)

add_library(dep INTERFACE)
target_link_libraries(dep INTERFACE SDL3::SDL3)
target_include_directories(dep INTERFACE ${CMAKE_CURRENT_LIST_DIR}/SDL/include)