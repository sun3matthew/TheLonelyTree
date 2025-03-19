#pragma once

#include <engine/component.h>
#include <engine/ui_component.h>
#include <engine/ui_render_object.h>

// * This is a pretty bad way of doing this.
class UICanvas : public Component{
    public:
        UICanvas();
        ~UICanvas() override;

        void update() override;

        void addUIRenderObject(UIRenderObject* uiRenderObject);
        void removeUIRenderObject(UIRenderObject* uiRenderObject);
        
        void addUIComponent(UIComponent* uiComponent);
        void removeUIComponent(UIComponent* uiComponent);

        UIComponent* getUIComponent(int index);
        UIComponent* getCurrentFocused();
    private:
        std::vector<UIComponent*> UIComponents;
        UIComponent* currentFocused;
};
