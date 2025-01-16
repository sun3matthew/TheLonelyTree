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
        
        std::vector<glm::vec3> positions;

        Grass(std::vector<PrimitiveVertex> vertices);
        // TODO add destructor

        void drawCall(Shader* shader) override;
    private:
        //  render data
        unsigned int VAO, VBO, instanceVBO;

        void setupMesh();
};  

#endif