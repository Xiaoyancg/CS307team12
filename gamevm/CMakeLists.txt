file(GLOB srcs "src/*.cpp")

add_library(gamevm ${srcs})

target_link_libraries(gamevm core glad imgui sdlmain glm)

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm>
)
add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/gamevm/src/fragment.frag
            ${CMAKE_CURRENT_BINARY_DIR}/fragment.frag)
add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/gamevm/src/vertex.vert
            ${CMAKE_CURRENT_BINARY_DIR}/vertex.vert)