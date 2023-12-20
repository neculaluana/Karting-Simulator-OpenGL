#include <GL/glew.h>
#include <glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include <vector>

void processInput(GLFWwindow* window);

unsigned int LoadSkybox(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                data);
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

int main() {
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW window hints here (before creating the window)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Karting Simulator Prejmer", NULL, NULL);
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
    glfwMakeContextCurrent(window);

    // Set up framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create Camera
    Camera* camera= new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 400.0f, 300.0f));
    
    // Setup Skybox
    std::vector<std::string> faces= {
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\right.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\left.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\top.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\bottom.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\front.jpg)",
        "C:\\G3D_Project\\Karting Simulator\\Karting Simulator\\back.jpg)"
        
    };
    Skybox skybox(faces, camera);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    // Main loop
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
       
        // Clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT); // Clear depth buffer before drawing skybox
        glDepthFunc(GL_LEQUAL);
        // Render the skybox
        glDepthMask(GL_FALSE); // Disable depth write for skybox drawing

        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        glm::mat4 projection = camera->GetProjectionMatrix();

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
        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();



    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
