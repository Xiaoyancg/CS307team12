file(GLOB srcs "src/*.cpp")

add_library(GameVM ${srcs})

target_include_directories(GameVM
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(GameVM glad imgui sdlmain glm)

add_custom_command (TARGET GameVM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:GameVM>
)

if(WITH_TESTS)
	file(GLOB test_srcs "test/*.cpp")
	add_executable(TestGameVM ${test_srcs})
	target_link_libraries(TestGameVM GameVM gtest_main)
	gtest_discover_tests(TestGameVM)
endif(WITH_TESTS)