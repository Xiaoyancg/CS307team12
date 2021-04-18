# core CMAKELIST
file(GLOB srcs "src/*.cpp")
file(GLOB headers "include/*.h")

add_library(corelib ${srcs} ${headers})
#target_compile_options( corelib PRIVATE /w)
target_include_directories(corelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(corelib gamevmlib sdl glad sdlmain imgui nlohmann_json::nlohmann_json glm soil freetype)

# Core exe
add_executable(core "CoreMain.cpp" ${srcs} ${headers})

target_compile_options(core PRIVATE /w)
target_include_directories(core
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(core sdl glad sdlmain imgui glm nlohmann_json::nlohmann_json soil freetype)

add_custom_command (TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core>
)
