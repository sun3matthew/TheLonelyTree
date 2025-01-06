#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <engine/shader.h>

class Light{
    public:
        Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
        virtual ~Light() = default;

        virtual void writeToShader(Shader& shader) = 0;

    protected:
        glm::vec3 ambient, diffuse, specular;
};

#endif