#include "Skybox.h";

Skybox::Skybox(const std::vector<std::string>& faces) {
	skyboxShader = new Shader("skyboxVertexShader.vs", "skyboxFragmentShader.fs");
	loadTextures(faces);
	setupMesh();
}

Skybox::~Skybox() {
	delete skyboxShader;
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
	glDepthFunc(GL_LEQUAL);

	// Use the shader program
	glUseProgram(skyboxShader->GetID());

	// Set uniforms for view and projection matrices
	glUniformMatrix4fv(skyboxShader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(skyboxShader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};