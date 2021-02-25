file(GLOB srcs "src/*.cpp")

add_executable(gamevm ${srcs})

target_link_libraries(gamevm glad imgui sdlmain)
target_include_directories(gamevm PRIVATE "${GLM_DIR}")

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
