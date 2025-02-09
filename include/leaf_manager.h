#ifndef LEAF_MANAGER_H
#define LEAF_MANAGER_H

#include <vector>
#include <list>
#include <engine/render_object.h>

struct Leaf{
    glm::vec3 point;
    glm::vec3 direction;
    int type;
};

struct LeafKey{
    unsigned int LeafType;
    unsigned int LeafIdx;
};

struct LeafSet{
    unsigned int VAO, VBO;
    std::vector<Leaf> leafData;
};

class LeafManager : public RenderObject{
public:
    static std::vector<Texture> leafTextures;

    LeafManager();
    ~LeafManager();

    LeafKey getNewLeafKey();
    void writeLeafData(LeafKey key, glm::vec3 point, glm::vec3 direction);

    void drawCall(Shader* shader) override;
    void writeDataToGPU();
private:
    std::vector<LeafSet*> leafSets;
};

#endif