#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include<string>

class Shader {
public:
    GLuint Program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();
    
private:

    bool CheckCompileErrors(GLuint shader, std::string type);
};

#endif