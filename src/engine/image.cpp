#include <engine/image.h>
#include <glad/glad.h>
#include <engine/glfw_wrapper.h>

Image::Image(Texture* texture, glm::vec2 min, glm::vec2 max, glm::vec3 color)
    : texture(texture), color(color)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    setPosition(min, max);
}

Image::~Image()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Image::drawCall(Shader* shader)
{
    UIRenderObject::drawCall(shader);

    shader->setVec3("color", color);

    texture->bind(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Image::updateTexture(Texture* texture){
    this->texture = texture;
}

void Image::setPosition(glm::vec2 min, glm::vec2 max)
{
    glm::vec2 scale = glm::vec2(GLFWWrapper::width, GLFWWrapper::height);

    this->min = min;
    this->max = max;

    float vertices[] = {
        min.x * scale.x, min.y * scale.y, 0.0f, 0.0f,
        min.x * scale.x, max.y * scale.y, 0.0f, 1.0f,
        max.x * scale.x, max.y * scale.y, 1.0f, 1.0f,

        min.x * scale.x, min.y * scale.y, 0.0f, 0.0f,
        max.x * scale.x, max.y * scale.y, 1.0f, 1.0f,
        max.x * scale.x, min.y * scale.y, 1.0f, 0.0f
    };


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Image::setColor(glm::vec3 color){
    this->color = color;
}