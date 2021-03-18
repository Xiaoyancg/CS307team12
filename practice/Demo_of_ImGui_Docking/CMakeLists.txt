add_executable(demo_imgui_docking "main.cpp" "imgui_demo.cpp" )
target_link_libraries(demo_imgui_docking imgui glm imfb glad sdl sdlmain)

add_custom_command (TARGET demo_imgui_docking POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:demo_imgui_docking>
)

