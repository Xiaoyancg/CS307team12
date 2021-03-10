file(GLOB srcs "src/*.cpp")
file(GLOB headers "include/*.h")

add_library(gamevmlib ${srcs}${headedrs})

target_include_directories(gamevmlib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(gamevmlib corelib editorlib glad imgui sdlmain glm nlohmann_json)

add_executable(gamevm "GameVMMain.cpp")

target_link_libraries(gamevm gamevmlib)

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm>
)

