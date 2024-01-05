#ifndef MESH_CLASS_H
#define MESH_CLASS_H
#define MAX_BONE_INFLUENCE 4
#pragma once
#include<string>
#include"Camera.h"
#include<vector>
#include"Texture.h"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:
	std::shared_ptr <Vertex> vertices;
	std::shared_ptr <GLuint> indices;
	std::vector <Texture> textures;
	GLuint VAO;
	unsigned int numVertices;
	unsigned int numIndexes;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	Mesh(unsigned int numVertices, std::shared_ptr <Vertex> vertices, unsigned int numIndexes, std::shared_ptr <unsigned int> indices,  std::vector<Texture>& textures);

	void Draw(Shader& shader);
private:
	GLuint VBO;
	GLuint EBO;
	void setupMesh();
};
#endif
