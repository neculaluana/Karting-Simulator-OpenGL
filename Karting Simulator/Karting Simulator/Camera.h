#include <GL/glew.h>

#include <GLM.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float Ka = 0.1;
float Kd = 0.1;
float Ks = 0.1;
float expKs = 2;
float radius = 1.0;

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
	const float cameraSpeedFactor = 2.5f;
	const float mouseSensitivity = 1.0f;

	float zNear;
	float zFar;
	float FoVy;
	int width;
	int height;
	bool isPerspective;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.f, lastY = 0.f;
public:
	Camera(const int width, const int height, const glm::vec3& position);
};