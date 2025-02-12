// #ifndef UI_RENDER_OBJECT_H
// #define UI_RENDER_OBJECT_H

// #include <engine/render_object.h>

// class UIRenderObject : public RenderObject
// {
// public:
//     UIRenderObject();
//     ~UIRenderObject();

//     void drawCall(Shader*) override;
// private:
//     unsigned int VAO, VBO;
// };

// #endif // UI_RENDER_OBJECT_H

#include <engine/ui_render_object.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
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