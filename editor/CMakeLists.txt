file(GLOB srcs "src/*.cpp")

add_executable(Editor ${srcs})

target_link_libraries(editor gamevm sdlmain imfb)

add_custom_command (TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:Editor>
)
