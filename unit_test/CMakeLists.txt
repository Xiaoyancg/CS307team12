set (CORE_DIR "${PROJECT_SOURCE_DIR}/core")
file(GLOB core_srcs "${CORE_DIR}/src/*.cpp")
file(GLOB core_headers "${CORE_DIR}/include/*.h")

add_library(core_test ${core_srcs} ${core_headers})
target_compile_definitions(core_test PUBLIC __CORE_TEST)
target_include_directories(core_test
	PUBLIC
		$<BUILD_INTERFACE:${CORE_DIR}/include>
		$<INSTALL_INTERFACE:${CORE_DIR}/include>
	PRIVATE
		${CORE_DIR}/src
)
target_link_libraries(core_test sdl glad sdlmain imgui glm testlib)

add_custom_command (TARGET core_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core_test>
)

if(WITH_TESTS)
	file(GLOB test_src "src/*.cpp")
	file(GLOB test_header "include/*.h")
	add_executable(test_core ${test_src} ${test_header})

target_include_directories(test_core 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
	target_link_libraries(test_core core_test gtest_main)
	gtest_discover_tests(test_core)
endif(WITH_TESTS)
