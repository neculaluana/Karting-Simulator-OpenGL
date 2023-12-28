#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

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
