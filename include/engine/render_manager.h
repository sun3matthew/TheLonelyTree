#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

// ! Ok I do not know how to design this to be not specific to the game.
// TODO if you ever want to tackle that, make it so that you can dynamically add shaders and lights/etc know what to add to which shaders.

#include <unordered_map>
#include <vector>
#include <string>

#include <engine/shader.h>
#include <engine/render_object.h>

#define BASE_FAME_BUFFER "base"
#define FRAME_BUFFER "frameBuffer"
#define SHADOW_BUFFER "shadowBuffer"


struct FrameBuffer{
    std::string name;
    std::vector <Texture> textures;
    unsigned int framebuffer;
    float width, height;
    int clearFlags;
};

class RenderManager{
    public:
        static RenderManager instance;

        RenderManager();
        ~RenderManager();

        // TODO add cache
        std::vector<Shader*> getShadersAccepting(std::string attribute);

        void addFrameBuffer(FrameBuffer);
        void updateFrameBuffer(int i, int width, int height);
        FrameBuffer getFrameBuffer(int);
        FrameBuffer getFrameBuffer(std::string);

        void addShader(Shader*);
        Shader* getShader(std::string);

        void addToBuffer(RenderObject*);
        void addToBuffer(std::string, RenderObject*);

        void draw(int);
        void draw(std::string);
        void draw();

        void clearBuffer();
    private:
        std::unordered_map<std::string, Shader*> shaders;
        
        // TODO render orders
        std::unordered_map<std::string, int> renderOrder;
        std::vector<FrameBuffer> frameBuffers;
        std::vector<std::vector<RenderObject*>> renderBuffer;

        void activateFrameBuffer(int);
};

#endif