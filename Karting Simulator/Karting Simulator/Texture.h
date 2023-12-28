#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#pragma once
#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	std::string type;
	GLuint unit;
	std::string path;
	Texture(const char* image, const char* texType, GLuint slot);
	Texture();
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	
	void Unbind();
	
	void Delete();
};
#endif