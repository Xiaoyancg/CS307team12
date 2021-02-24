file(GLOB srcs "src/*.cpp")

add_executable(gamevm ${srcs})

target_link_libraries(gamevm glad imgui sdlmain)
target_include_directories(gamevm PRIVATE "${GLM_DIR}")

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm>
)

