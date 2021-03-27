file(GLOB srcs "src/*.cpp")

add_executable(demo_editor ${srcs})

target_link_libraries(demo_editor sdl imgui glad sdlmain)
target_include_directories(demo_editor PRIVATE "${GLM_DIR}")

add_custom_command (TARGET demo_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:demo_editor>
)
add_custom_command (TARGET demo_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/src/fragment.frag
            ${CMAKE_CURRENT_BINARY_DIR}/fragment.frag)
add_custom_command (TARGET demo_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/src/vertex.vert
            ${CMAKE_CURRENT_BINARY_DIR}/vertex.vert)

