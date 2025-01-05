#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>
#include <engine/camera.h>

class TheLonelyTree : public GLFWWrapper{
    public:
        TheLonelyTree();
        ~TheLonelyTree();
    protected:
        void start() override;
        void update() override;

    private:
        Camera camera;
};  

#endif