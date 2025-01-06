#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include <engine/mesh.h>

class GLTFLoader{
    public:
        static Mesh* loadMesh(const char* path);
};

#endif