#include "Camera.h"

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true)
{
	yaw += xOffset;
	pitch += yOffset;
	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	//UpdateCameraVectors();
}
