cmake_minimum_required(VERSION 3.10)
project(MyGLFWApp)

set(CMAKE_CXX_STANDARD 17)

# Add GLFW (từ source)
add_subdirectory(vendors/glfw)

# Tạo executable
add_executable(my_app src/main.cpp)

# Thêm include dirs
target_include_directories(my_app
    PUBLIC
        vendors/glfw/include
        vendors               # <- Để trình biên dịch thấy vendors/glad/gl.h
)

# Link GLFW và OpenGL
target_link_libraries(my_app glfw)

# # Link OpenGL
# find_package(OpenGL REQUIRED)
# target_link_libraries(my_app OpenGL::GL)

# # Needed on Linux for glad
# target_link_libraries(my_app dl)

