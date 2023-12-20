#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)

Shader::~Shader()
{
    glDeleteProgram(ID);
}

unsigned int Shader::GetID() const
{
    return ID;
}


void Shader::CreateShaders(void) {

    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
    glCompileShader(VertexShaderId);
    CheckCompileErrors(VertexShaderId, "VERTEX");


    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after compiling vertex shader: " << err << std::endl;
    }


    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
    glCompileShader(FragmentShaderId);
    CheckCompileErrors(FragmentShaderId, "FRAGMENT");


    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after compiling fragment shader: " << err << std::endl;
    }
}


void Shader::DestroyShaders(void)
{
    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);

    glDeleteProgram(ProgramId);
}

void Shader::Init(const char* vertexPath, const char* fragmentPath) {
    CreateShaders();

    ProgramId = glCreateProgram();
    if (ProgramId == 0) {
        std::cerr << "Failed to create shader program" << std::endl;
        return;
    }

    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    glLinkProgram(ProgramId);

    GLint isLinked = 0;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(ProgramId, maxLength, &maxLength, &infoLog[0]);
        std::cerr << "Shader Linking Error: " << &infoLog[0] << std::endl;

        glDeleteProgram(ProgramId);
        ProgramId = 0; // Set to zero to indicate failure
        return;
    }

    glUseProgram(ProgramId);

    // Retrieve uniform locations
    glUniform1i(glGetUniformLocation(ProgramId, "skybox"), 0);
    loc_view_matrix = glGetUniformLocation(ProgramId, "view");
    loc_projection_matrix = glGetUniformLocation(ProgramId, "projection");

    if (loc_view_matrix == -1) {
        std::cerr << "Warning: Uniform 'view' location not found." << std::endl;
    }
    if (loc_projection_matrix == -1) {
        std::cerr << "Warning: Uniform 'projection' location not found." << std::endl;
    }

    // Detach and delete shaders as they are no longer needed
    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);
    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);
}



bool Shader::CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- "
                << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- "
                << std::endl;
            return false;
        }
    }
    return true;
}