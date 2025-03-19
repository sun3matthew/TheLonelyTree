#pragma once

#include <engine/component.h>
#include <engine/render_object_component.h>
#include <engine/ui_render_object.h>

class UIComponent : public RenderObjectComponent{
    public:
        UIComponent(UIRenderObject* renderObject);
        ~UIComponent() override;

        void update() override;

        void setCanvas(UICanvas* canvas);

        void setClickable(bool clickable);
        bool isClickable();

        UIRenderObject* getUIRenderObject();

        virtual void onClick();
        virtual void hovering(bool hover);
        virtual void focused();
        virtual void onUnfocused();
    protected:
        UICanvas* canvas;

        bool clickable;
};