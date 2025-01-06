#include <engine/light_directional.h>

LightDirectional::LightDirectional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
    : Light(amb, diff, spec), direction(dir)
{}

void LightDirectional::writeToShader(Shader& shader){
    shader.setVec3("dirLight.direction", direction.x, direction.y, direction.z);
    shader.setVec3("dirLight.lightingData.ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("dirLight.lightingData.diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.setVec3("dirLight.lightingData.specular", specular.x, specular.y, specular.z);
}