#ifndef GRASS_H
#define GRASS_H

#include <glm/glm.hpp>

#include <engine/shader.h>
#include <engine/texture.h>
#include <engine/render_object.h>

#include <vector>
#include <string>

struct PrimitiveVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Grass : public RenderObject{
    public:
        // mesh data
        std::vector<PrimitiveVertex> vertices;
        Grass();
        // TODO add destructor

        void drawCall(Shader* shader) override;
    private:
        //  render data
        unsigned int VAO, VBO;

        void setupMesh();
};  

#endif