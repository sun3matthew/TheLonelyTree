#ifndef MESH_COMP_H
#define MESH_COMP_H

#include <engine/component.h>
#include <engine/mesh.h>

class MeshComp : public Component{
    public:
        MeshComp(Mesh*);
        ~MeshComp() override;

        void update() override;

        void updateMesh(Mesh*);

        Mesh* getMesh();
    private:
        Mesh* mesh;
};

#endif