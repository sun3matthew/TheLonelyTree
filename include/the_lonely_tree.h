#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>
#include <engine/gameobject.h>
#include <engine/camera.h>

#include <vector>
#include <list>

class TheLonelyTree : public GLFWWrapper{
    public:
        TheLonelyTree* instance;

        TheLonelyTree();
        ~TheLonelyTree() override;
    protected:
        void start() override;
        void update() override;
        void lateUpdate() override;

    private:
        Camera* camera;
        int numLights;
};  

#endif