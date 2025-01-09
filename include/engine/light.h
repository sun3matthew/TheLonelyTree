#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <engine/shader.h>
#include <engine/component.h>

class Light : public Component
{
    public:
        Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
    protected:
        glm::vec3 ambient, diffuse, specular;
};

#endif