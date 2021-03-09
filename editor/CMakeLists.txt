file(GLOB srcs "src/*.cpp")

add_executable(editor ${srcs})

target_link_libraries(editor GameVM sdlmain imfb sdl imgui)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor>
)
