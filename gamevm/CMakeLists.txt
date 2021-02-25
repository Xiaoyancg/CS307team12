file(GLOB srcs "src/*.cpp")

add_library(gamevm ${srcs})

target_link_libraries(gamevm core)
