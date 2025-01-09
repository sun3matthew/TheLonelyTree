#include <engine/light_directional.h>
#include <engine/render_manager.h>
#include <vector>

LightDirectional::LightDirectional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
    : Light(amb, diff, spec), direction(dir)
{}

void LightDirectional::update(){
    std::vector<Shader*> shaders = RenderManager::instance.getShadersAccepting("dirLight");
    for(Shader* shader : shaders){
        shader->setVec3("dirLight.direction", direction.x, direction.y, direction.z);
        shader->setVec3("dirLight.lightingData.ambient", ambient.x, ambient.y, ambient.z);
        shader->setVec3("dirLight.lightingData.diffuse", diffuse.x, diffuse.y, diffuse.z);
        shader->setVec3("dirLight.lightingData.specular", specular.x, specular.y, specular.z);
    }
}