enum class EDirection {
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down,
};

class Camera {
private:
	const float zNEAR = 0.1f;
	const float zFAR = 500.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();
protected:

public:

};