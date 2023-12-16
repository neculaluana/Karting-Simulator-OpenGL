#include "Shader.h"

Shader::Shader() {
    // Initialize with default behavior or leave empty if nothing is needed
}


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    Init(vertexPath, fragmentPath);
}

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
    glAttachShader(ProgramId, VertexShaderId);

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
        return;
    }


    glValidateProgram(ProgramId);
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after shader program validation: " << err << std::endl;
    }
    glUseProgram(ProgramId);
    glUniform1i(glGetUniformLocation(ProgramId, "skybox"), 0);
    loc_view_matrix = glGetUniformLocation(ProgramId, "view");
    loc_projection_matrix = glGetUniformLocation(ProgramId, "projection");

    if (loc_view_matrix == -1) {
        std::cerr << "Warning: Uniform 'view' location not found." << std::endl;
    }
    if (loc_projection_matrix == -1) {
        std::cerr << "Warning: Uniform 'projection' location not found." << std::endl;
    }

    glAttachShader(ProgramId, FragmentShaderId);


    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);
    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);


}


void Shader::CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }

}