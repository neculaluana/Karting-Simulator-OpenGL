#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/glew.h>
#include <glfw3.h>
#include <GLM.hpp>
#include <vector>
#include <string>
#include "Shader.h"
#include "Camera.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces, Camera* camera);
    Skybox();
    void render(const glm::mat4& view, const glm::mat4& projection);
    GLuint VAO, VBO;
    GLuint textureID;
    ~Skybox();

private:
    Shader* skyboxShader;
    std::vector<std::string> faces;
    unsigned int loadTextures(const std::vector<std::string>& faces);
    void setupMesh();
    Camera* camera;
};
#endif