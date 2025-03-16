#ifndef RENDER_OBJECT_COMPONENT_H
#define RENDER_OBJECT_COMPONENT_H

#include <engine/component.h>
#include <engine/render_object.h>

class RenderObjectComponent : public Component{
    public:
        RenderObjectComponent(RenderObject*);
        ~RenderObjectComponent() override;

        void update() override;

        void updateRenderObject(RenderObject*);

        RenderObject* getRenderObject();
    protected:
        RenderObject* renderObject;
};

#endif