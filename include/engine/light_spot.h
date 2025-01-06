#ifndef LIGHT_SPOT_H
#define LIGHT_SPOT_H

#include <engine/light.h>

class LightSpot : public Light{
    public:
        LightSpot(glm::vec3 pos, glm::vec3 att, unsigned int idx, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

        void writeToShader(Shader& shader) override;

    private:
        glm::vec3 position, attenuation;
        unsigned int index;
};

#endif
