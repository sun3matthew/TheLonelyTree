
// class MeshComp : public Component{
//     public:
//         MeshComp(Gameobject*, Mesh*);
//         ~MeshComp() override;

//         void update() override;

//         void updateMesh(Mesh*);
//     private:
//         Mesh* mesh;
// };

#include <engine/mesh_comp.h>
#include <engine/render_manager.h>
#include <engine/gameobject.h>

MeshComp::MeshComp(Mesh* meshIn)
    : mesh(meshIn)
{}

MeshComp::~MeshComp(){
    if(mesh)
        delete mesh;
}

void MeshComp::update(){
    mesh->modelMatrix = gameobject->getModelMatrix();
    RenderManager::instance.addToBuffer(mesh);
}

void MeshComp::updateMesh(Mesh* newMesh){
    if(mesh)
        delete mesh;
    mesh = newMesh;
}

Mesh* MeshComp::getMesh(){
    return mesh;
}
