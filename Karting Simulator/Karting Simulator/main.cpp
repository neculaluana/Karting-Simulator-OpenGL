#include <GL/glew.h>
#include <glfw3.h>
#include "Skybox.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main() {

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Skybox Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    Camera camera(800, 600, glm::vec3(0.0f, 0.0f, 0.0f));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

   
    std::vector<std::string> faces = {
        "Resources/right.jpg",
        "Resources/left.jpg",
        "Resources/top.jpg",
        "Resources/bottom.jpg",
        "Resources/front.jpg",
        "Resources/back.jpg"
    };

    
    Skybox skybox(faces);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
 
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        skybox.render(view, projection);

   
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}