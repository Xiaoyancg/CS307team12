# core CMAKELIST
file(GLOB srcs "src/*.cpp")

add_library(corelib ${srcs})
target_include_directories(corelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(corelib PUBLIC sdl glad nlohmann_json::nlohmann_json glm soil freetype)

# Core exe
add_executable(core "CoreMain.cpp")
target_link_libraries(core corelib sdlmain)
add_custom_command (TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:core>
)
