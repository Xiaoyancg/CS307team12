# editor
file(GLOB_RECURSE srcs "src/*.cpp")
file(GLOB_RECURSE headers "include/*.h")

add_library(editorlib ${srcs} ${headers}  "include/windows/SpriteSheetWindow.h")
#target_compile_options(editorlib PRIVATE /w)
target_include_directories(editorlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(editorlib corelib gamevmlib sdlmain imfb sdl imgui glm)

add_executable(editor WIN32 "EditorMain.cpp" "include/windows/SpriteSheetWindow.h")
#target_compile_options( editor PRIVATE /w )
target_link_libraries(editor editorlib)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor>
)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resource
    $<TARGET_FILE_DIR:editor>
)
