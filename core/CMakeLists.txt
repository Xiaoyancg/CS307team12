file(GLOB srcs "src/*.cpp")

#add_library(core ${srcs})
add_executable(core ${srcs} "include/Page.h" "src/Entity.cpp" "include/Entity.h")


target_include_directories(core 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(core sdl glad sdlmain imgui)

add_custom_command (TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core>
)

