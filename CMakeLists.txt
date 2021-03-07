cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Parchment VERSION 0.1 LANGUAGES CXX)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

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

set(IMFB_DIR "${PROJECT_SOURCE_DIR}/lib/imgui-filepath")
add_library(imfb INTERFACE)
target_include_directories(imfb INTERFACE ${IMFB_DIR})

set(GLAD_DIR "${PROJECT_SOURCE_DIR}/lib/glad")
add_library(glad "${GLAD_DIR}/src/glad.cpp")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

add_subdirectory(core)
add_subdirectory(gamevm)
add_subdirectory(editor)
