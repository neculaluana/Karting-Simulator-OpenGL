#include <GL/glew.h>
#include <glfw3.h>
#include "Skybox.h"
#include "Camera.h"
#include "Shader.h"  // Include your Shader class

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}



int main() {
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW window hints here (before creating the window)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Skybox Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Error check after GLEW initialization
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after GLEW initialization: " << err << std::endl;
    }
    // Set the viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Set up framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create Camera
    Camera camera(800, 600, glm::vec3(0.0f, 0.0f, 0.0f));

    // Setup Skybox
    std::vector<std::string> faces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    Skybox skybox(faces);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Main loop
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        // Poll for and process events
        glfwPollEvents();

        // Clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT); // Clear depth buffer before drawing skybox
        glDepthFunc(GL_LEQUAL);
        // Render the skybox
        glDepthMask(GL_FALSE); // Disable depth write for skybox drawing

        glm::mat4 skyboxView = camera.GetSkyboxViewMatrix(); // Remove translation
        glm::mat4 projection = camera.GetProjectionMatrix();

        skybox.render(skyboxView, projection);
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        // ... (Render other objects here)
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error at the end of the rendering loop: " << err << std::endl;
        }
        glfwSwapBuffers(window);
    }



    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
