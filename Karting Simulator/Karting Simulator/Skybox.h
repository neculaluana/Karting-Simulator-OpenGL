#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <GLM.hpp>
#include <vector>
#include <string>
#include "Shader.h"
#include "Camera.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void render(const glm::mat4& view, const glm::mat4& projection);
    ~Skybox();

private:
    GLuint VAO, VBO;
    GLuint textureID;
    Shader* skyboxShader;
    std::vector<std::string> faces;
    void loadTextures(const std::vector<std::string>& faces);
    void setupMesh();
};






