#ifndef LIGHT_DIRECTIONAL_H
#define LIGHT_DIRECTIONAL_H

#include <engine/light.h>
#include <engine/camera.h>

class LightDirectional : public Light{
    public:
        LightDirectional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);

        void update() override;
        void setWorldSize(float worldSize){this->worldSize = worldSize;}

    private:
        float angle;
        glm::vec3 direction;

        float worldSize;
};

#endif
