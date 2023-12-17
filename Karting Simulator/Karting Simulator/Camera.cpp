#include "Camera.h"

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	yaw += xOffset;
	pitch += yOffset;
	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward.y = sin(glm::radians(pitch));
	this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward = glm::normalize(this->forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

Camera::Camera(const int width, const int height, const glm::vec3& position):
	width(width), height(height), startPosition(position)
{
	Set(width, height, position);
}

void Camera::Set(const int width, const int height, const glm::vec3& position)
{
	this->width = width;
	this->height = height; 
	this->position = position;
	isPerspective = true;
	this->yaw = YAW;
	this->pitch = PITCH;
	this->zNear = zNEAR;
	this->zFar = zFAR;
	this->FoVy = FOV;

	lastX = width / 2.0f;
	lastY = height / 2.0f;

	// Set the camera to look towards the negative Z-axis
	this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0, 1, 0);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
	this->worldUp = up;

	// Update the view and projection matrices
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetSkyboxViewMatrix() const {
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f), forward, up);
	return glm::mat4(glm::mat3(view)); // Remove translation for the skybox
}


// Function to get the projection matrix
glm::mat4 Camera::GetProjectionMatrix() const {
	glm::mat4 Proj = glm::mat4(1);
	if (isPerspective)
	{
		float aspectRatio = ((float)(width)) / height;
		Proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
	}
	else
	{
		float scaleFactor = 2000.f;
		Proj = glm::ortho<float>(
			-width / scaleFactor, width / scaleFactor,
			-height / scaleFactor, height / scaleFactor, -zFar, zFar);
	}
	return Proj;
}