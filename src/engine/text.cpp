#include <engine/text.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/glfw_wrapper.h>

Text::Text(Font* font, std::string text, glm::vec2 position, float scale, glm::vec3 color)
    : font(font), text(text), position(position), scale(scale), color(color)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
}

Text::~Text()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Text::drawCall(Shader* shader)
{
    shader->use();
    shader->setVec3("textColor", color);

    glBindVertexArray(VAO);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(GLFWWrapper::width), 0.0f, static_cast<float>(GLFWWrapper::height));
    shader->setMat4("projection", projection);

    std::string::const_iterator c;
    glm::vec2 currentPosition = position;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = font->Characters[*c];

        float xpos = currentPosition.x + ch.Bearing.x * scale;
        float ypos = currentPosition.y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        // ! UNSUPPORTED (log once): POSSIBLE ISSUE: unit 0 GLD_TEXTURE_INDEX_2D is unloadable and bound to sampler type (Float) - using zero texture because texture unloadable
        shader->setTexture(ch.texture, 0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        currentPosition.x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
}

void Text::updateFont(Font* font){
    this->font = font;
}

void Text::setText(std::string text){
    this->text = text;
}

void Text::setPosition(glm::vec2 position){
    this->position = position;
}

void Text::setColor(glm::vec3 color){
    this->color = color;
}

void Text::setScale(float scale){
    this->scale = scale;
}