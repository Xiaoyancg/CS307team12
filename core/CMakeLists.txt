file(GLOB srcs "src/*.cpp")

#add_library(core ${srcs})

#add_executable(core ${srcs} "include/Page.h" "include/Game.h" )
add_library(Core ${srcs} "include/Page.h" "include/Game.h"  "include/testGame.h")

#target_include_directories(core 
#	PUBLIC
#		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#		$<INSTALL_INTERFACE:include>
#	PRIVATE
#		${CMAKE_CURRENT_SOURCE_DIR}/src
#)

target_include_directories(Core 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)


#target_link_libraries(core sdl glad sdlmain imgui)
target_link_libraries(Core sdl glad sdlmain imgui)

#add_custom_command (TARGET core POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core>
#)

if(WITH_TESTS)
	file(GLOB test_srcs "test/*.cpp")
	add_executable(TestGame ${test_srcs} "include/testGame.h")
target_include_directories(TestGame 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
	target_link_libraries(TestGame Core gtest_main)
	gtest_discover_tests(TestGame)
endif(WITH_TESTS)
