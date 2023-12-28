#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H
#pragma once
#include<json/json.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"Mesh.h"
#include<stb/stb_image.h>
using json = nlohmann::json;
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
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

	
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
#endif