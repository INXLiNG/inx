add_executable(sandbox)
target_sources(sandbox PRIVATE
    src/main.cpp
)

target_link_libraries(sandbox PRIVATE ${PROJECT_NAME})