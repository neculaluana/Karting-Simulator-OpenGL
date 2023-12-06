#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "Shader.h"

class Terrain {
public:
    Terrain(int width, int height, const std::string& heightmapPath, float scale = 1.0f);
    ~Terrain();

    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO, EBO;
    GLuint textureID;
    Shader* terrainShader;
    int width;
    int height;
    float scale;
    std::vector<std::vector<float>> heightmap;

    void loadHeightmap(const std::string& heightmapPath);
    void generateTerrain();
    void setupMesh();
};
