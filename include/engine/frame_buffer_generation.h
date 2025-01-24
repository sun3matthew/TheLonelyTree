#ifndef FRAME_BUFFER_GENERATION_H
#define FRAME_BUFFER_GENERATION_H

#include <engine/render_manager.h>
#include <glad/glad.h> 

class FrameBufferGeneration {
public:
    static FrameBuffer ShadowMap(float width, float height, unsigned int depthMap){
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);  

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  

        return {depthMapFBO, width, height, GL_DEPTH_BUFFER_BIT};
    }

private:
    FrameBufferGeneration() = delete;
};

#endif // FRAME_BUFFER_GENERATION_H
