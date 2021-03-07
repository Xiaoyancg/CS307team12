set (CORE_DIR "${PROJECT_SOURCE_DIR}/core")
file(GLOB core_srcs "${CORE_DIR}/*.cpp")
file(GLOB core_headers "${CORE_DIR}/*.h")

add_library(core_test ${core_srcs} ${core_headers})

#add_executable(core ${srcs} "include/Page.h" "include/Game.h" )
#add_library(Core ${srcs} "include/Page.h" "include/Game.h"  "include/testGame.h")

#target_include_directories(core 
#	PUBLIC
#		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#		$<INSTALL_INTERFACE:include>
#	PRIVATE
#		${CMAKE_CURRENT_SOURCE_DIR}/src
#)

target_include_directories(core_test
	PUBLIC
		$<BUILD_INTERFACE:${CORE_DIR}/include>
		$<INSTALL_INTERFACE:${CORE_DIR}/include>
	PRIVATE
		${CORE_DIR}/src
)

target_link_libraries(core_test sdl glad sdlmain imgui glm)


add_custom_command (TARGET core_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core_test>
)

#if(WITH_TESTS)
#	file(GLOB test_srcs "test/*.cpp")
#	add_executable(TestGame ${test_srcs} "include/testGame.h")
#target_include_directories(TestGame 
#	PUBLIC
#		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#		$<INSTALL_INTERFACE:include>
#	PRIVATE
#		${CMAKE_CURRENT_SOURCE_DIR}/src
#)
#	target_link_libraries(TestGame Core gtest_main)
#	gtest_discover_tests(TestGame)
#endif(WITH_TESTS)
