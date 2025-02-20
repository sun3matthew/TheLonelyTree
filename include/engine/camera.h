#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/component.h>
#include <engine/shader.h>

// Defines several possible options for camera movement.
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 200.5f;
const float SENSITIVITY = 0.4f;
const float ZOOM = 45.0f;

class Camera : public Component{
public:
    glm::vec3 originPosition;
    // Camera Attributes
    glm::vec3 Front, Up, Right, WorldUp;
    // Euler Angles
    float Yaw, Pitch;
    // Camera options
    float MovementSpeed, MouseSensitivity, Zoom;
    float time;

    // Constructors
    Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    Camera(float upX, float upY, float upZ,
           float yaw, float pitch);

    // View matrix
    glm::mat4 GetViewMatrix();

    // Input processing
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void update() override;
private:
    // Update vectors based on Euler Angles
    void updateCameraVectors();
};

#endif
