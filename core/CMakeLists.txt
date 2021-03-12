file(GLOB srcs "src/*.cpp")
file(GLOB headers "include/*.h")

add_library(corelib ${srcs} ${headers} )

target_include_directories(corelib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(corelib editorlib gamevmlib sdl glad sdlmain imgui glm nlohmann_json::nlohmann_json)

add_executable(core "CoreMain.cpp" )

target_include_directories(core
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(core corelib)

add_custom_command (TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core>
)
