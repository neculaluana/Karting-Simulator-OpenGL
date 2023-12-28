#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"Mesh.h"

using json = nlohmann::json;
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	Model(const char* file);
	Model(std::string const& path, bool bSmoothNormals, bool gamma=false);

	void Draw(Shader& shader);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;
	std::string directory;
	bool gammaCorrection;
	void loadModel(const std::string& path, bool bSmoothNormals);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	void loadMesh(unsigned int indMesh);

	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	//std::vector<Vertex> assembleVertices
	//(
	//	std::vector<glm::vec3> positions,
	//	std::vector<glm::vec3> normals,
	//	std::vector<glm::vec2> texUVs
	//);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
#endif