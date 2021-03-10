set (CORE_DIR "${PROJECT_SOURCE_DIR}/core")
set (VM_DIR "${PROJECT_SOURCE_DIR}/gamevm")
set (EDITOR_DIR "${PROJECT_SOURCE_DIR}/editor")

file(GLOB editor_srcs "${CORE_DIR}/src/*.cpp")
file(GLOB editor_headers "${CORE_DIR}/include/*.h")
file(GLOB vm_srcs "${VM_DIR}/src/*.cpp")
file(GLOB vm_headers "${VM_DIR}/include/*.h")
file(GLOB core_srcs "${EDITOR_DIR}/src/*.cpp")
file(GLOB core_headers "${EDITOR_DIR}/include/*.h")

add_library(core_test ${core_srcs} ${core_headers})

target_compile_definitions(core_test PUBLIC __TEST_CORE)
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



add_library(editor_test ${editor_srcs} ${editor_headers})

target_compile_definitions(editor_test PUBLIC __TEST_EDITOR)
target_include_directories(editor_test
	PUBLIC
		$<BUILD_INTERFACE:${EDITOR_DIR}/include>
		$<INSTALL_INTERFACE:${EDITOR_DIR}/include>
	PRIVATE
		${EDITOR_DIR}/src
)
target_link_libraries(editor_test sdl glad sdlmain imgui glm testlib)

add_custom_command (TARGET editor_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:editor_test>
)


add_library(gamevm_test ${gamevm_srcs} ${gamevm_headers})

target_compile_definitions(gamevm_test PUBLIC __TEST_VM)
target_include_directories(gamevm_test
	PUBLIC
		$<BUILD_INTERFACE:${VM_DIR}/include>
		$<INSTALL_INTERFACE:${VM_DIR}/include>
	PRIVATE
		${VM_DIR}/src
)
target_link_libraries(gamevm_test sdl glad sdlmain imgui glm testlib)

add_custom_command (TARGET gamevm_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm_test>
)


if(WITH_TESTS)
	file(GLOB test_src "src/*.cpp")
	file(GLOB test_header "include/*.h")
	add_executable(unittest ${test_src} ${test_header})

target_include_directories(unittest 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
	target_link_libraries(unittest core_test gtest_main)
	gtest_discover_tests(unittest)
endif(WITH_TESTS)
