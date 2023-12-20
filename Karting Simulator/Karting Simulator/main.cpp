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
    Shader skyboxShader("C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\skyboxVertexShader.vs", "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\skyboxFragmentShader.fs");

    // Load skybox
    Skybox skybox;
    std::vector<std::string> faces = {
        "C:\\Resources\\right.jpg",
        "C:\\Resources\\left.jpg",
        "C:\\Resources\\top.jpg",
        "C:\\Resources\\bottom.jpg",
        "C:\\Resources\\front.jpg",
        "C:\\Resources\\back.jpg"
    };
    skybox.loadTextures(faces);

    // Create camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw skybox
        skyboxShader.Use();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        skybox.render(skyboxShader.Program, view, projection);

        // GLFW: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GLFW: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
