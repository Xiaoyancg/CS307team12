file(GLOB srcs "src/*.cpp")

add_executable(demo_gamevm ${srcs})

target_link_libraries(demo_gamevm glad imgui sdlmain sdl glm nlohmann_json::nlohmann_json soil )
target_include_directories(demo_gamevm PRIVATE "${GLM_DIR}")

add_custom_command (TARGET demo_gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:demo_gamevm>
)

#add_custom_command (TARGET demo_gamevm POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy
#            ${CMAKE_SOURCE_DIR}/demo_gamevm/src/fragment.frag
#            ${CMAKE_BINARY_DIR}/myexe/fragment.frag)
#add_custom_command (TARGET demo_gamevm POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy
#            ${CMAKE_SOURCE_DIR}/demo_gamevm/src/vertex.vert
#            ${CMAKE_BINARY_DIR}/myexe/vertex.vert)
