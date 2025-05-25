cmake_minimum_required(VERSION 3.10)
project(minecraftC C)

# Collect source files explicitly or with globbing
file(GLOB SOURCES "src/*.c")

# Add executable target
add_executable(minecraftC ${SOURCES})

target_link_libraries(minecraftC m)
# Add include directories (just directory, not files)
target_include_directories(minecraftC PRIVATE inc)