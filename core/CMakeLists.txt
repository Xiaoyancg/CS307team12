file(GLOB srcs "src/*.cpp")

add_executable(Core ${srcs})

target_include_directories(Core 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Core sdl glad imgui)
