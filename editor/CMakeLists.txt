file(GLOB srcs "src/*.cpp")
file(GLOB header "include/*.h")

add_library(editorlib ${srcs} ${header} )

target_link_libraries(editorlib corelib sdlmain imfb sdl imgui)

target_include_directories(editorlib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(editor "EditorMain.cpp")

target_link_libraries(editor editorlib)

add_custom_command (TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor>
)




