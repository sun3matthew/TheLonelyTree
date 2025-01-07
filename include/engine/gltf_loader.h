#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include <engine/mesh.h>
#include <vector>

class GLTFLoader{
    public:
        static std::vector<Mesh> loadMesh(const char* path);
};

#endif