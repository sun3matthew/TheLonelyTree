
#include <engine/light.h>

Light::Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
    : ambient(amb), diffuse(diff), specular(spec)
{}