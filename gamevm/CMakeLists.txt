# gamevm
file(GLOB srcs "src/*.cpp")

add_library(gamevmlib ${srcs})
target_include_directories(gamevmlib PUBLIC	${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gamevmlib corelib)

add_executable(gamevm "GameVMMain.cpp")
target_link_libraries(gamevm gamevmlib sdlmain)

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sdl> $<TARGET_FILE_DIR:gamevm>
)

add_custom_command (TARGET gamevm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/gdata
    $<TARGET_FILE_DIR:gamevm>
)
