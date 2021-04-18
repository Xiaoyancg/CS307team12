# Root CMAKELIST
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Parchment VERSION 0.1 LANGUAGES CXX)

#set(CMAKE_CXX_FLAGS_T "${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /IGNORE:4005")

#if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
#	string(REGEX REPLACE "/W[0-4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")	
#else()
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
#endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	

message("${CMAKE_CXX_FLAGS}")

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

option(WITH_TESTS "Build tests" ON)
if(WITH_TESTS)
	enable_testing()
	include(GoogleTest)
	set(gtest_force_shared_crt ON)
	add_subdirectory(lib/googletest)
endif()

add_link_options($<$<CXX_COMPILER_ID:MSVC>:/ignore:4099>)

set(SDL_DIR "${PROJECT_SOURCE_DIR}/lib/SDL")
add_library(sdl SHARED IMPORTED)
set_property(TARGET sdl PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL_DIR}/include")
set_property(TARGET sdl PROPERTY IMPORTED_LOCATION "${SDL_DIR}/SDL2.dll")
set_property(TARGET sdl PROPERTY IMPORTED_IMPLIB "${SDL_DIR}/SDL2.lib")
add_library(sdlmain STATIC IMPORTED)
set_property(TARGET sdlmain PROPERTY IMPORTED_LOCATION "${SDL_DIR}/SDL2main.lib")

set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/lib/imgui")
file(GLOB imgui_srcs "${IMGUI_DIR}/*.cpp")
set(IMGUI_BACKENDS_DIR "${IMGUI_DIR}/backends")
list(APPEND imgui_srcs
	"${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp"
	"${IMGUI_BACKENDS_DIR}/imgui_impl_sdl.cpp"
)
add_library(imgui ${imgui_srcs})
target_link_libraries(imgui sdl glad)
target_include_directories(imgui
	PUBLIC
		"${IMGUI_DIR}"
		"${IMGUI_BACKENDS_DIR}"
)

set(GLM_DIR "${PROJECT_SOURCE_DIR}/lib/glm")
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${GLM_DIR})
target_compile_options(glm INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/wd5033 /wd4996>)

set(IMFB_DIR "${PROJECT_SOURCE_DIR}/lib/imgui-filebrowser")
add_library(imfb INTERFACE)
target_include_directories(imfb INTERFACE ${IMFB_DIR})

set(GLAD_DIR "${PROJECT_SOURCE_DIR}/lib/glad")
add_library(glad "${GLAD_DIR}/src/glad.cpp")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")
target_compile_options(glad PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/wd4005>)

set(SOIL_DIR "${PROJECT_SOURCE_DIR}/lib/SOIL")
add_library(soil STATIC IMPORTED)
set_property(TARGET soil PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SOIL_DIR}/include")
set_property(TARGET soil PROPERTY IMPORTED_LOCATION "${SOIL_DIR}/SOIL.lib")
target_link_libraries(soil INTERFACE opengl32)

set(FREETYPE_DIR "${PROJECT_SOURCE_DIR}/lib/Freetype")
add_library(freetype STATIC IMPORTED)
set_property(TARGET freetype PROPERTY IMPORTED_LOCATION "${FREETYPE_DIR}/freetype.lib")
set_property(TARGET freetype PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_DIR}/include")

add_subdirectory(lib/json)

# set output locations to one place
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/myarc)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/mylib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/myexe)
add_subdirectory(core)
add_subdirectory(gamevm)
add_subdirectory(editor)
add_subdirectory(unit_test)

# practice
add_subdirectory(practice/Demo_of_ImGui_Docking)
add_subdirectory(practice/Demo_of_engine)
add_subdirectory(practice/Demo_of_ImGui)