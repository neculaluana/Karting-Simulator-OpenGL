#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<vector>

class Shader {
public:
    GLuint Program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();
    
private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif