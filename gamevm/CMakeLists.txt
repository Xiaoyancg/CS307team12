file(GLOB srcs "src/*.cpp")

add_library(gamevm ${srcs})

#target_link_libraries(gamevm core glad imgui sdlmain glm)
target_link_libraries(gamevm glad imgui sdlmain glm)


add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm>
)