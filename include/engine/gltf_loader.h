#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include <engine/gameobject.h>

class GLTFLoader{
    public:
        static Gameobject* loadMesh(const char* path);
};

#endif