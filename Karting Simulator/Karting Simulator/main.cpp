
#include<filesystem>

namespace fs = std::filesystem;


#include"Model.h"
#include<stb/stb_image.h>

const unsigned int width = 1920;
const unsigned int height = 1080;

//bool firstPerson = false;
Camera* pCamera = nullptr;

void Cleanup()
{
	delete pCamera;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// timing
double deltaTime = 0.0f;    // time between current frame and last frame
double lastFrame = 0.0f;


float skyboxVertices[] =
{

	-100.0f, -100.0f,  100.0f,
	 100.0f, -10.0f,  100.0f,
	 100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f, -100.0f,
	-100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f
};

unsigned int skyboxIndices[] =
{

	1, 2, 6,
	6, 5, 1,

	0, 4, 7,
	7, 3, 0,

	4, 5, 6,
	6, 7, 4,

	0, 3, 2,
	2, 1, 0,

	0, 1, 5,
	5, 4, 0,

	3, 7, 6,
	6, 2, 3
};


glm::vec3 kartPosition = glm::vec3(12.5f, -18.5f, -5.0f);
bool firstPerson = false;

int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Karting Simulator", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);





	Shader shader("defaultVertexShader.vs", "defaultFragmentShader.fs");
	Shader skyboxShader("skyboxVertexShader.vs", "skyboxFragmentShader.fs");
	Shader lampShader("Lamp.vs", "Lamp.fs");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shader.Activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);





	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));






	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::string facesCubemap[6] =
	{
		 "Resources/right.jpg",
		"Resources/left.jpg",
		 "Resources/top.jpg",
		 "Resources/bottom.jpg",
		"Resources/front.jpg",
		"Resources/back.jpg"
	};

	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		//if(i==0) stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	std::string kartObjFileName = "Resources/Models/Kart4/GoCart.obj";

	Model kartObjModel(kartObjFileName.c_str(), false);
	/*glm::mat4 kartModelMatrix = glm::mat4(1.0f);
	kartModelMatrix = glm::translate(kartModelMatrix, glm::vec3(0.0f, -2.0f, -5.0f));
	kartModelMatrix = glm::scale(kartModelMatrix, glm::vec3(1.0f));*/

	glm::mat4 kartModelMatrix = glm::mat4(1.0f);
	kartModelMatrix = glm::translate(kartModelMatrix, glm::vec3(12.5f, -18.5f, -5.0f));
	kartModelMatrix = glm::rotate(kartModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate by 180 degrees around the y-axis
	kartModelMatrix = glm::scale(kartModelMatrix, glm::vec3(1.0f));


	std::string trackObjFileName = "Resources/Models/Track/10605_Slot_Car_Race_Track_v1_L3.obj";
	Model trackObjModel(trackObjFileName.c_str(), false);
	//glm::mat4 trackModelMatrix = glm::mat4(1.0f);
	//trackModelMatrix = glm::translate(trackModelMatrix, glm::vec3(0.0f, -20.0f, -5.0f));
	//float rotationAngle = glm::radians(90.0f); // For a 90-degree rotation
	//trackModelMatrix = glm::rotate(trackModelMatrix, rotationAngle, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotating around the X axis

	//trackModelMatrix = glm::scale(trackModelMatrix, glm::vec3(1.0f));

	glm::mat4 trackModelMatrix = glm::mat4(1.0f);
	trackModelMatrix = glm::translate(trackModelMatrix, glm::vec3(0.0f, -20.0f, 30.0f));
	float rotationAngle = glm::radians(90.0f);
	trackModelMatrix = glm::rotate(trackModelMatrix, rotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	float rotationAngle2 = glm::radians(180.0f);
	trackModelMatrix = glm::rotate(trackModelMatrix, rotationAngle2, glm::vec3(0.0f, 1.0f, 0.0f));
	trackModelMatrix = glm::scale(trackModelMatrix, glm::vec3(1.0f));

	// Move the track forward by adjusting the translation
	trackModelMatrix = glm::translate(trackModelMatrix, glm::vec3(0.0f, -40.0f, 0.0f)); // Adjust the Z component

	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 3.0));

	// Set the callback functions
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Enable cursor capture (optional, but might be useful for a first-person camera)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glm::mat4 projection, view;
	
	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightPos.x = 0.5 * cos(glfwGetTime());
		lightPos.z = 0.5 * sin(glfwGetTime());

		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_FRONT);
		skyboxShader.Activate();

		view = glm::mat4(glm::mat3(pCamera->GetViewMatrix()));
		projection = pCamera->GetProjectionMatrix();

		// Apply a translation to the skybox based on the camera's position
		glm::mat4 skyboxModelMatrix = glm::translate(glm::mat4(1.0f), pCamera->GetPosition());

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK); // Reset to default culling mode

		float kartSpeed = 0.05f;
		
		
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			pCamera->SetPosition(kartPosition + glm::vec3(0.0f, 0.0f, 3.0f));
			firstPerson = true;
		}

		/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			kartModelMatrix = glm::translate(kartModelMatrix, kartSpeed * glm::vec3(0.0f, 0.0f, 1.0f));
			kartPosition = kartSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
			if (firstPerson)
			{
				pCamera->SetPosition(kartPosition);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			kartModelMatrix = glm::translate(kartModelMatrix, kartSpeed * glm::vec3(1.0f, 0.0f, 0.0f));
			kartModelMatrix = glm::rotate(kartModelMatrix, glm::radians(0.75f), glm::vec3(0.0f, 1.0f, 0.0f));
			kartPosition = kartSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
			if (firstPerson)
			{
				pCamera->SetPosition(kartPosition);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			kartModelMatrix = glm::translate(kartModelMatrix, kartSpeed * glm::vec3(0.0f, 0.0f, -1.0f));
			kartPosition = kartSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
			if (firstPerson)
			{
				pCamera->SetPosition(kartPosition);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			kartModelMatrix = glm::translate(kartModelMatrix, kartSpeed * glm::vec3(-1.0f, 0.0f, 0.0f));
			kartModelMatrix = glm::rotate(kartModelMatrix, glm::radians(-0.75f), glm::vec3(0.0f, 1.0f, 0.0f));
			kartPosition = kartSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
			if (firstPerson)
			{
				pCamera->SetPosition(kartPosition);
			}
		}*/
		
		//float kartSpeed = 0.05f;
		glm::vec3 direction(0.0f);
		float rotationAngle =glm::radians(0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			direction.z = kartSpeed; // Assuming -Z is forward
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			direction.z = -kartSpeed; // Assuming +Z is backward
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			direction.x = kartSpeed; // Assuming -X is left
			rotationAngle = glm::radians(0.75f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			direction.x = -kartSpeed; // Assuming +X is right
			rotationAngle = glm::radians(-0.75f);
		}

		kartModelMatrix = glm::translate(kartModelMatrix, direction);
		//kartModelMatrix = glm::rotate(kartModelMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

		kartPosition += direction;

		// Update camera position if in firstPerson mode
		if (firstPerson) {
			glm::vec3 cameraOffset = glm::vec3(0.0f, 1.0f, -3.0f); // Offset the camera position
			glm::vec3 cameraByKartPosition(0.0f);
			cameraByKartPosition.x = -kartPosition.x+25.0f;
			cameraByKartPosition.z = -kartPosition.z-6.75f;
			cameraByKartPosition.y = kartPosition.y-0.38f;
			pCamera->SetPosition(cameraByKartPosition + cameraOffset);
		}
		
		// Draw the kart with the updated position and rotation
		//kartModelMatrix = glm::translate(kartModelMatrix, kartPosition);
		//kartModelMatrix = glm::rotate(kartModelMatrix, glm::radians(kartRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//kartModelMatrix = glm::scale(kartModelMatrix, glm::vec3(1.0f));

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(kartModelMatrix));
		kartObjModel.Draw(shader);

		glDisable(GL_CULL_FACE);
		shader.Activate();
		 view = glm::mat4(1.0f);
		 projection = glm::mat4(1.0f);
		view = pCamera->GetViewMatrix();
		projection = pCamera->GetProjectionMatrix();
		
		glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), 1.0f, 0.0f, 0.0);
		glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, &pCamera->GetPosition()[0]);

		
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(kartModelMatrix));
		kartObjModel.Draw(shader);



		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		shader.Activate();
		view = pCamera->GetViewMatrix();
		projection = pCamera->GetProjectionMatrix();
		glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), 0.6f, 0.6f, 0.6f);
		glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, &pCamera->GetPosition()[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(trackModelMatrix));
		trackObjModel.Draw(shader);



		glDepthFunc(GL_LEQUAL);

		//lampShader.Activate();
		//view = pCamera->GetViewMatrix();
		//projection = pCamera->GetProjectionMatrix();
		//glUniformMatrix4fv(glGetUniformLocation(lampShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(lampShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
		//lightModel = glm::scale(lightModel, glm::vec3(0.05f)); // a smaller cube
		//glUniformMatrix4fv(glGetUniformLocation(lampShader.ID, "model"), 1, GL_FALSE, &lightModel[0][0]);
		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// Activate the skybox shader
		
		/*
		skyboxShader.Activate();

		view = pCamera->GetViewMatrix();
		projection = pCamera->GetProjectionMatrix();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));



		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);*/


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	shader.Delete();
	skyboxShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Update camera based on user input
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);
		firstPerson = false;
	}
	
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}
