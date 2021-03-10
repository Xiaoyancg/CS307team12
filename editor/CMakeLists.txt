file(GLOB srcs "src/*.cpp")
file(GLOB header "include/*.h")


add_library(editorlib ${srcs} ${header} )
target_link_libraries(editorlib sdlmain imfb sdl imgui)

add_executable(editor ${srcs})




add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor>
)




