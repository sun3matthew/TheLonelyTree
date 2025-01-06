#include <engine/camera.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
      Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch) {
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
      Position(glm::vec3(posX, posY, posZ)),
      WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch) {
    updateCameraVectors();
}

// View matrix
glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

// Keyboard input
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

// Mouse movement
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

// Mouse scroll
void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

// Update camera vectors
void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::writeToShader(Shader &shader){
    glm::mat4 view = GetViewMatrix();
    shader.setMat4("view", glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    shader.setMat4("projection", glm::value_ptr(projection));

    // std::cout << Position.x << std::endl;
    shader.setVec3("viewPos", Position.x, Position.y, Position.z);
}