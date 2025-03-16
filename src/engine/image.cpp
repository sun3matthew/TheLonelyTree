#include <engine/image.h>
#include <glad/glad.h>
#include <engine/glfw_wrapper.h>

Image::Image(Texture* texture, glm::vec2 min, glm::vec2 max, glm::vec3 color)
    : texture(texture)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    setColor(color);
    setPosition(min, max);
}

Image::~Image()
{
}

void Image::drawCall(Shader* shader)
{
    UIRenderObject::drawCall(shader);

    shader->setVec3("color", color);
    texture->bind(0);

    glm::vec2 scale = glm::vec2(GLFWWrapper::width, GLFWWrapper::height);

    glm::vec2 min = aabb.min;
    glm::vec2 max = aabb.max;

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

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Image::updateTexture(Texture* texture){
    this->texture = texture;
}