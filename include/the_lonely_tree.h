#ifndef THE_LONELY_TREE_H
#define THE_LONELY_TREE_H

#include <engine/glfw_wrapper.h>

class TheLonelyTree : public GLFWWrapper{
    public:
        ~TheLonelyTree();
    protected:
        void start() override;
        void update() override;
};  

#endif