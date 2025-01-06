#ifndef LIGHT_DIRECTIONAL_H
#define LIGHT_DIRECTIONAL_H

#include <engine/light.h>

class LightDirectional : public Light{
    public:
        LightDirectional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

        void writeToShader(Shader& shader) override;

    private:
        glm::vec3 direction;
};

#endif
