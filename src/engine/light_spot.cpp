#include <engine/light_spot.h>
#include <string>

LightSpot::LightSpot(glm::vec3 pos, glm::vec3 att, unsigned int idx, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
    : Light(amb, diff, spec), position(pos), attenuation(att), index(idx)
{}

void LightSpot::writeToShader(Shader& shader){
    shader.setVec3("pointLights[" + std::to_string(index) + "].position", position.x, position.y, position.z);
    shader.setVec3("pointLights[" + std::to_string(index) + "].attenuation", attenuation.x, attenuation.y, attenuation.z);
    shader.setVec3("pointLights[" + std::to_string(index) + "].lightingData.ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("pointLights[" + std::to_string(index) + "].lightingData.diffuse", diffuse.x, diffuse.y, diffuse.z); 
    shader.setVec3("pointLights[" + std::to_string(index) + "].lightingData.specular", specular.x, specular.y, specular.z);
}