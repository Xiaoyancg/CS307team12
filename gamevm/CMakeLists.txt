# gamevm
file(GLOB srcs "src/*.cpp")
file(GLOB headers "include/*.h")

add_library(gamevmlib ${srcs} ${headers})
#target_compile_options(gamevmlib PRIVATE /w)
target_include_directories(gamevmlib PUBLIC	${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gamevmlib corelib editorlib glad imgui sdlmain nlohmann_json)

add_executable(gamevm "GameVMMain.cpp")
#target_compile_options(gamevm PRIVATE /w)
target_link_libraries(gamevm gamevmlib)

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm>
)

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/gdata
    $<TARGET_FILE_DIR:editor>
)
