# editor
file(GLOB_RECURSE srcs "src/*.cpp")

add_library(editorlib ${srcs})
target_include_directories(editorlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(editorlib PUBLIC corelib gamevmlib sdl imgui imfb glm)

#windres ../resource/parchment.rc -o app.o
add_executable(editor "EditorMain.cpp" "../resource/parchment.rc")
target_link_libraries(editor editorlib sdlmain)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor>
)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resource
    $<TARGET_FILE_DIR:editor>
)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/gdata
    $<TARGET_FILE_DIR:editor>
)
