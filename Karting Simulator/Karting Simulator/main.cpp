#include <GL/glew.h>
#include <glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include <vector>
#include <iostream>

void processInput(GLFWwindow* window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Skybox", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Build and compile shaders
    Shader skyboxShader("path/to/skyboxVertexShader.vert", "path/to/skyboxFragmentShader.frag");

    // Load skybox
    Skybox skybox;
    std::vector<std::string> faces = {
        "path/to/right.jpg",
        "path/to/left.jpg",
        "path/to/top.jpg",
        "path/to/bottom.jpg",
        "path/to/front.jpg",
        "path/to/back.jpg"
    };
    skybox.loadTextures(faces);

    // Create camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Setup Skybox
    std::vector<std::string> faces= {
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\right.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\left.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\top.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\bottom.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\front.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\back.jpg)"
        
    };
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
