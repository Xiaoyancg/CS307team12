file(GLOB srcs "src/*.cpp")

add_executable(editor ${srcs})

target_link_libraries(editor sdl imgui glad sdlmain)
target_include_directories(editor PRIVATE "${GLM_DIR}")

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor>
)
add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/editor/src/fragment.frag
            ${CMAKE_CURRENT_BINARY_DIR}/fragment.frag)
add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/editor/src/vertex.vert
            ${CMAKE_CURRENT_BINARY_DIR}/vertex.vert)

