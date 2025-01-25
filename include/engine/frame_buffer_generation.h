#ifndef FRAME_BUFFER_GENERATION_H
#define FRAME_BUFFER_GENERATION_H

#include <engine/render_manager.h>
#include <glad/glad.h> 

class FrameBufferGeneration {
public:
    static FrameBuffer BaseFrameBuffer(std::string name, float width, float height){
        unsigned int gBuffer;
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        Texture colorTexture = Texture(width, height, GL_RGB, GL_UNSIGNED_BYTE, TextureType::FrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.getID(), 0);

        Texture depthTexture = Texture(width, height, GL_DEPTH_COMPONENT, GL_FLOAT, TextureType::DepthBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.getID(), 0);

        // Check framebuffer status
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return {name, {colorTexture, depthTexture}, gBuffer, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT};
    }

    static FrameBuffer ShadowMap(std::string name, float width, float height){
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);  

        Texture shadowMap = Texture(width, height, GL_DEPTH_COMPONENT, GL_FLOAT, TextureType::DepthBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap.getID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  

        return {name, {shadowMap}, depthMapFBO, width, height, GL_DEPTH_BUFFER_BIT};
    }
private:
    FrameBufferGeneration() = delete;
};

#endif // FRAME_BUFFER_GENERATION_H
