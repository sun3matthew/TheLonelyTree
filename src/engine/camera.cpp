#include <engine/camera.h>
#include <glm/gtc/type_ptr.hpp>
#include <engine/render_manager.h>
#include <engine/gameobject.h>

// Constructor with vectors
Camera::Camera(glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
      WorldUp(up), Yaw(yaw), Pitch(pitch) {
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float upX, float upY, float upZ,
               float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
      WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch) {
    updateCameraVectors();
}

// View matrix
glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 position = gameobject->getPosition();
    return glm::lookAt(position, position + Front, Up);
}

// Keyboard input
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    glm::vec3 position = gameobject->getPosition();
    if (direction == FORWARD)
        position += Front * velocity;
    if (direction == BACKWARD)
        position -= Front * velocity;
    if (direction == LEFT)
        position -= Right * velocity;
    if (direction == RIGHT)
        position += Right * velocity;
    gameobject->setPosition(position);
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

void Camera::update(){
    float near_plane = 10.0f, far_plane = 5000.0f;

    glm::mat4 view = GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(Zoom), 800.0f / 600.0f, near_plane, far_plane);

    std::vector<Shader*> shaders = RenderManager::instance.getShadersAccepting("camera");
    glm::vec3 position = gameobject->getPosition();

    for(Shader* shader : shaders){
        shader->use();
        shader->setMat4("view", glm::value_ptr(view));
        shader->setMat4("projection", glm::value_ptr(projection));
        shader->setVec3("viewPos", position.x, position.y, position.z);
    }

    shaders = RenderManager::instance.getShadersAccepting("cameraSkyBox");
    for(Shader* shader : shaders){
        glm::mat4 viewSkyBox = glm::mat4(glm::mat3(view));  
        shader->use();
        shader->setMat4("view", glm::value_ptr(viewSkyBox));
        shader->setMat4("projection", glm::value_ptr(projection));
    }

    shaders = RenderManager::instance.getShadersAccepting("screenSpaceData");
    for(Shader* shader : shaders){
        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("invProjection", glm::inverse(projection));
        shader->setFloat("near", near_plane);
        shader->setFloat("far", far_plane);
    }

}