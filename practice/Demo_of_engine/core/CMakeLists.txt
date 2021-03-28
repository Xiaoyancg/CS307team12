file(GLOB srcs "src/*.cpp")

add_library(demo_core ${srcs})

target_include_directories(demo_core 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(demo_core sdl glad imgui)
