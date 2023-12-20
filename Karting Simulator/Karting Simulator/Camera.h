#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position, Front, Up;
    Camera(glm::vec3 position);
    glm::mat4 GetViewMatrix();
};

#endif