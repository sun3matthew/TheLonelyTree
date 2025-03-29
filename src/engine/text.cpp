#include <engine/text.h>
#include <glad/glad.h>
#include <engine/glfw_wrapper.h>

Text::Text(Font* font, std::string text, glm::vec2 min, glm::vec2 max, float scale, glm::vec3 color)
    : font(font), text(text), alignment(TextAlignment::LOWER_LEFT), cursorPosition(-1), cursorVisible(false)
{
    setColor(color);
    setPosition(min, max);
    setScale(scale);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

Text::~Text()
{}

AABB getAlignmentAABB(TextAlignment alignment, AABB aabb){
    glm::vec2 min = aabb.minPoint;
    glm::vec2 max = aabb.maxPoint;

    // bottom left is 0, 0
    switch (alignment){
        case TextAlignment::UPPER_LEFT:
            return AABB(glm::vec2(min.x, max.y), glm::vec2(max.x, min.y));
        // case TextAlignment::UPPER_CENTER:
        case TextAlignment::UPPER_RIGHT:
            return AABB(glm::vec2(max.x, max.y), glm::vec2(min.x, min.y));
        // case TextAlignment::MIDDLE_LEFT:
        // case TextAlignment::MIDDLE_CENTER:
        // case TextAlignment::MIDDLE_RIGHT:
        case TextAlignment::LOWER_LEFT:
            return AABB(glm::vec2(min.x, min.y), glm::vec2(max.x, max.y));
        // case TextAlignment::LOWER_CENTER:
        case TextAlignment::LOWER_RIGHT:
            return AABB(glm::vec2(max.x, min.y), glm::vec2(min.x, max.y));
        default:
            return AABB(glm::vec2(min.x, max.y), glm::vec2(max.x, min.y));
    }
}

void Text::drawCall(Shader* shader)
{
    UIRenderObject::drawCall(shader);

    shader->setVec3("textColor", color);

    glBindVertexArray(VAO);

    for(int i = 0; i < characterAABBs.size(); i++){
        Character ch = font->Characters[text[i]];

        float vertices[] = {
            characterAABBs[i].minPoint.x, characterAABBs[i].maxPoint.y, 0.0f, 0.0f,
            characterAABBs[i].minPoint.x, characterAABBs[i].minPoint.y, 0.0f, 1.0f,
            characterAABBs[i].maxPoint.x, characterAABBs[i].minPoint.y, 1.0f, 1.0f,

            characterAABBs[i].minPoint.x, characterAABBs[i].maxPoint.y, 0.0f, 0.0f,
            characterAABBs[i].maxPoint.x, characterAABBs[i].minPoint.y, 1.0f, 1.0f,
            characterAABBs[i].maxPoint.x, characterAABBs[i].maxPoint.y, 1.0f, 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        shader->setTexture(ch.texture, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }

    if (cursorVisible){
        // TODO make this better.
        glm::vec2 currentPosition = getAlignmentAABB(alignment, aabb).minPoint * glm::vec2(GLFWWrapper::width, GLFWWrapper::height);
        float lineSpacing = 0.03 * GLFWWrapper::height;
        if(alignment == TextAlignment::UPPER_LEFT || alignment == TextAlignment::UPPER_RIGHT){
            currentPosition.y -= (lineSpacing) * scale;
        }

        if (cursorPosition != 0){
            char c = text[cursorPosition - 1];
            Character ch = font->Characters[c];
            float advance = (ch.Advance >> 6) * scale;
            if (c == '\n'){
                advance = 0;
            }
            currentPosition = glm::vec2(
                characterAABBs[cursorPosition - 1].minPoint.x + advance,
                characterAABBs[cursorPosition - 1].minPoint.y
            );
        }

        Character ch = font->Characters['|'];

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        AABB aabb = AABB(
            glm::vec2(currentPosition.x, currentPosition.y),
            glm::vec2(currentPosition.x + w, currentPosition.y + h)
        );

        float vertices[] = {
            aabb.minPoint.x, aabb.maxPoint.y, 0.0f, 0.0f,
            aabb.minPoint.x, aabb.minPoint.y, 0.0f, 1.0f,
            aabb.maxPoint.x, aabb.minPoint.y, 1.0f, 1.0f,

            aabb.minPoint.x, aabb.maxPoint.y, 0.0f, 0.0f,
            aabb.maxPoint.x, aabb.minPoint.y, 1.0f, 1.0f,
            aabb.maxPoint.x, aabb.maxPoint.y, 1.0f, 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        shader->setTexture(ch.texture, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }


    glBindVertexArray(0);
}

void Text::setCursorPosition(int position){
    cursorPosition = position;
}

void Text::setCursorVisible(bool visible){
    cursorVisible = visible;
}

void Text::setPosition(glm::vec2 min, glm::vec2 max){
    UIRenderObject::setPosition(min, max);
    recalculateCache();
}

void Text::setScale(float scale){
    UIRenderObject::setScale(scale);
    recalculateCache();
}

void Text::updateFont(Font* font){
    this->font = font;
    recalculateCache();
}

void Text::setAlignment(TextAlignment alignment){
    this->alignment = alignment;
    recalculateCache();
}

void Text::setText(std::string text){
    if (this->text == text)
        return;

    this->text = text;
    recalculateCache();
}

std::string Text::getText(){
    return text;
}

std::vector<AABB>& Text::getCharacterAABBs(){
    return characterAABBs;
}

void Text::recalculateCache(){
    characterAABBs.clear();

    std::vector<std::string> splitText;

    // split with whitespace
    std::string word = "";
    for (char c : text){
        if (c == ' ' || c == '\n'){
            splitText.push_back(word + c);
            word = "";
        }else{
            word += c;
        }
    }
    if (word != "")
        splitText.push_back(word);
        
    AABB textAABB = getAlignmentAABB(alignment, aabb);

    glm::vec2 currentPosition = textAABB.minPoint;
    glm::vec2 maxPosition = textAABB.maxPoint;

    currentPosition *= glm::vec2(GLFWWrapper::width, GLFWWrapper::height);
    maxPosition *= glm::vec2(GLFWWrapper::width, GLFWWrapper::height);

    float lineSpacing = 0.03 * GLFWWrapper::height;

    if(alignment == TextAlignment::UPPER_LEFT || alignment == TextAlignment::UPPER_RIGHT){
        currentPosition.y -= (lineSpacing) * scale;
    }


    for (std::string word : splitText){
        float wordWidth = 0;
        for (char c : word){
            Character ch = font->Characters[c];
            wordWidth += (ch.Advance >> 6) * scale;
        }

        if (currentPosition.x + wordWidth > maxPosition.x && wordWidth < maxPosition.x){ // make sure word fits
            currentPosition.x = aabb.minPoint.x * GLFWWrapper::width;
            currentPosition.y -= (lineSpacing) * scale;
        }

        for (char c : word){
            Character ch = font->Characters[c];

            if (currentPosition.x > maxPosition.x || c == '\n'){
                currentPosition.x = aabb.minPoint.x * GLFWWrapper::width;
                currentPosition.y -= (lineSpacing) * scale;
            }


            float xPos = currentPosition.x + ch.Bearing.x * scale;
            float yPos = currentPosition.y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            float advance = (ch.Advance >> 6) * scale;

            if (c == '\n'){
                w = 0;
                h = 0;
                advance = 0;
            }


            characterAABBs.push_back(AABB(
                glm::vec2(xPos, yPos),
                glm::vec2(xPos + w, yPos + h)
            ));

            currentPosition.x += advance;
        }
    }
}

