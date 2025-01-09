#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>
#include <engine/camera.h>
#include <engine/mesh.h>
#include <engine/shader.h>
#include <engine/light_directional.h>
#include <engine/light_spot.h>
#include <engine/gameobject.h>

#include <vector>
#include <list>

class TheLonelyTree : public GLFWWrapper{
    public:
        TheLonelyTree();
        ~TheLonelyTree() override;
    protected:
        void start() override;
        void update() override;
        void lateUpdate() override;

    private:
        Camera* camera;
};  

#endif