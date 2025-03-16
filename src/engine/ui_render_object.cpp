#include <engine/ui_render_object.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <engine/glfw_wrapper.h>

UIRenderObject::UIRenderObject()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

UIRenderObject::~UIRenderObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void UIRenderObject::drawCall(Shader* shader)
{
    shader->use();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(GLFWWrapper::width), 0.0f, static_cast<float>(GLFWWrapper::height));
    shader->setMat4("projection", projection);
}

void UIRenderObject::setPosition(glm::vec2 min, glm::vec2 max){
    this->aabb = AABB(min, max);
}

void UIRenderObject::setColor(glm::vec3 color){
    this->color = color;
}

void UIRenderObject::setScale(float scale){
    this->scale = scale;
}