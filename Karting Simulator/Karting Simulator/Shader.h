#pragma once
#include <GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<vector>



class Shader {
public:
    Shader();
    void CreateShaders();
    void DestroyShaders();
    void Init(const char* vertexPath, const char* fragmentPath);
    GLuint VertexShaderId;
    GLuint ProgramId;
    GLuint FragmentShaderId;
    const GLchar* VertexShader =
    {
       "#version 400\n"\
       "layout(location=0) in vec4 in_Position;\n"\
       "layout(location=1) in vec4 in_Color;\n"\
       "out vec4 ex_Color;\n"\
       "void main(void)\n"\
       "{\n"\
       "  gl_Position = in_Position;\n"\
       "  ex_Color = in_Color;\n"\
       "}\n"

    };
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    unsigned int GetID() const;
    unsigned int loc_model_matrix;
    unsigned int loc_view_matrix;
    unsigned int loc_projection_matrix;

    const GLchar* FragmentShader =
    {
       "#version 400\n"\
       "in vec4 ex_Color;\n"\
       "out vec4 out_Color;\n"\
       "void main(void)\n"\
       "{\n"\
       "  out_Color = ex_Color;\n"\
       "}\n"
    };
private:

    void CheckCompileErrors(unsigned int shader, std::string type);
    unsigned int ID;
};