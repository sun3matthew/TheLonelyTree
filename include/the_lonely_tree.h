#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>
#include <engine/camera.h>
#include <engine/mesh.h>
#include <engine/shader.h>
#include <engine/light_directional.h>

#include <vector>

class TheLonelyTree : public GLFWWrapper{
    public:
        TheLonelyTree();
        ~TheLonelyTree();
    protected:
        void start() override;
        void update() override;

    private:
        Camera camera;
        LightDirectional directionalLight;
        Shader* meshShader;

        std::vector<Mesh> meshes;
};  

#endif