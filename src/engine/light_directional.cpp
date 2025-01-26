#include <engine/light_directional.h>
#include <engine/render_manager.h>
#include <engine/gameobject.h>
#include <engine/glfw_wrapper.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

LightDirectional::LightDirectional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
    : Light(amb, diff, spec), direction(dir)
{
    angle = 3.1415 * 1.2;
}
void LightDirectional::update(){
    angle += 0.01 * GLFWWrapper::instance->getDeltaTime();
    // std::cout << angle << std::endl;
    direction = glm::vec3(cos(angle), sin(angle), 0);
    std::vector<Shader*> shaders = RenderManager::instance.getShadersAccepting("dirLight");
    for(Shader* shader : shaders){
        shader->use();
        shader->setVec3("dirLight.direction", direction.x, direction.y, direction.z);
        shader->setVec3("dirLight.lightingData.ambient", ambient.x, ambient.y, ambient.z);
        shader->setVec3("dirLight.lightingData.diffuse", diffuse.x, diffuse.y, diffuse.z);
        shader->setVec3("dirLight.lightingData.specular", specular.x, specular.y, specular.z);
    }

    float near_plane = 1.0f, far_plane = 3000.5f;
    glm::mat4 lightProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, near_plane, far_plane);

    glm::vec3 lookAt = glm::vec3(worldSize / 2, 300.0f, worldSize / 2);
    glm::vec3 lightPos = direction * 1500.0f;
    glm::mat4 lightView = glm::lookAt(lookAt - lightPos, lookAt, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView; 

    shaders = RenderManager::instance.getShadersAccepting("dirLightCamera");
    for(Shader* shader : shaders){
        shader->use();
        shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    }


    shaders = RenderManager::instance.getShadersAccepting("dirLightPosition");
    for(Shader* shader : shaders){
        shader->use();
        shader->setVec3("dirLightPosition", lookAt.x, lookAt.y, lookAt.z);
    }
}