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
};

class Grass : public RenderObject{
    public:
        // mesh data
        std::vector<PrimitiveVertex> vertices;
        std::vector<Texture> textures;

        Grass();
        ~Grass();

        void drawCall(Shader* shader) override;
    private:

        Texture* perlinLane;
        //  render data
        unsigned int VAO, VBO;
        float time;

        void setupMesh();
};  

#endif