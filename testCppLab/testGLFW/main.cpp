#include <GLFW/glfw3.h>
#include <iostream>

// Callback when press keyboard
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Callback when move the mouse
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << "Mouse moved to: " << xpos << ", " << ypos << std::endl;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Window on Linux", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Register callback
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw (OpenGL if can)
        // ...

        glfwSwapBuffers(window);
        glfwPollEvents();  // Event processing
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

