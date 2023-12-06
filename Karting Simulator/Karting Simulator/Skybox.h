#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <GLM.hpp>
#include <vector>
#include <string>

class Skybox {
public:
    Skybox();
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO;
    GLuint textureID;
    std::vector<std::string> faces;
    void loadTextures(const std::vector<std::string>& faces);
    void setupMesh();
};






