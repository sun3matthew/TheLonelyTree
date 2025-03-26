

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

#include <engine/gltf_loader.h>
#include <engine/render_object_component.h>
#include <engine/mesh.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string> 
#include <cassert>

const float* GetAttributeData(const tinygltf::Model &model, const tinygltf::Primitive &primitive, const std::string &attributeName) {
    if (primitive.attributes.find(attributeName) != primitive.attributes.end()) {
        const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at(attributeName)];
        const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
        return reinterpret_cast<const float *>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);
    }
    return nullptr;
}

const std::vector<Gameobject*> processMesh(tinygltf::Mesh &mesh, glm::mat4 &transform, tinygltf::Model &model, std::vector<std::vector<Texture>> &mats){
    std::vector<Gameobject*> gameobjects;

    int i = 0;
    for (const auto &primitive : mesh.primitives) {
        i++;
        std::vector<unsigned int> indices;
        if (primitive.indices >= 0) {
            const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

            const unsigned char *dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
            for (size_t i = 0; i < accessor.count; ++i) {
                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                    indices.push_back(((unsigned short *)dataPtr)[i]);
                } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                    indices.push_back(((unsigned int *)dataPtr)[i]);
                }
            }
        }

        std::vector<Vertex> vertices;
        const float *positionData = GetAttributeData(model, primitive, "POSITION");
        const float *normalData = GetAttributeData(model, primitive, "NORMAL");
        const float *textureMappingData = GetAttributeData(model, primitive, "TEXCOORD_0");
        if (positionData && normalData && textureMappingData){
            for (size_t i = 0; i < model.accessors[primitive.attributes.at("POSITION")].count; ++i) {
                Vertex vertex;

                vertex.Position.x = positionData[i * 3];
                vertex.Position.y = positionData[i * 3 + 1];
                vertex.Position.z = positionData[i * 3 + 2];

                // transform..
                glm::vec4 pos(vertex.Position, 1.0f);
                glm::vec4 transformed = transform * pos;
                glm::vec3 result(transformed.x, transformed.y, transformed.z);
                vertex.Position = result;


                vertex.Normal.x = normalData[i * 3];
                vertex.Normal.y = normalData[i * 3 + 1];
                vertex.Normal.z = normalData[i * 3 + 2];

                vertex.TexCoords.x = textureMappingData[i * 2];
                vertex.TexCoords.y = textureMappingData[i * 2 + 1];

                vertices.push_back(vertex);
            }
        }
        // TODO do a move
        // Extract indices
        
        Mesh* generatedMesh = new Mesh(vertices, indices, mats[primitive.material]);
        generatedMesh->textures.push_back(Texture::defaultShadow());

        Gameobject* gameobject = new Gameobject(mesh.name + " " + std::to_string(i));
        gameobject->addComponent(new RenderObjectComponent(generatedMesh));
        gameobjects.push_back(gameobject);
    }
    return gameobjects;
}

Gameobject* processNode(int nodeIdx, glm::mat4 transform, tinygltf::Model &model, std::vector<std::vector<Texture>> &mats){
    tinygltf::Node &node = model.nodes[nodeIdx];
    Gameobject* gameobject = new Gameobject(node.name);
    
    if (node.matrix.size() == 16){
        transform = transform * (glm::mat4)glm::make_mat4(node.matrix.data());
    }

    if (node.mesh >= 0) {
        tinygltf::Mesh &mesh = model.meshes[node.mesh];
        std::vector<Gameobject*> newGameobjects = processMesh(mesh, transform, model, mats); // Handle the mesh
        for(Gameobject* newGameobject : newGameobjects)
            newGameobject->setParent(gameobject);
    }

    // Recursively process child nodes
    for (int childIndex : node.children) {
        Gameobject* newGameobject = processNode(childIndex, transform, model, mats);
        newGameobject->setParent(gameobject);
    }
    return gameobject;
}

inline unsigned char floatToChar(float v) {
    return static_cast<unsigned char>(v + 0.5f);
}

inline unsigned char mix(unsigned char a, unsigned char b, float t) {
    return floatToChar(a * (1.0f - t) + b * t);
}

#define BASE_SPECULAR 0x0A


Gameobject* GLTFLoader::loadMesh(const char* path){
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    // bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
    }

    if (model.scenes.size() == 0){
        printf("No scenes in glTF files\n");
    }


    std::vector<std::vector<Texture>> materials;

    for (const auto& material : model.materials) {
        std::vector<Texture> currentMat;

        int baseColorTexIndex = material.pbrMetallicRoughness.baseColorTexture.index;
        if (baseColorTexIndex >= 0){
            tinygltf::Image &image = model.images[model.textures[baseColorTexIndex].source];

            assert(image.bits == 8);
            assert(image.component == 4);

            unsigned char* imagePtr = reinterpret_cast<unsigned char*>(image.image.data());
            currentMat.push_back(Texture(imagePtr, image.width, image.height, 4, TextureType::Diffuse));
        }


        int metallicRoughnessTexIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
        if (metallicRoughnessTexIndex >= 0){
            tinygltf::Image &image = model.images[model.textures[metallicRoughnessTexIndex].source];

            assert(image.bits == 8);
            assert(image.component == 4);

            unsigned int width = image.width;
            unsigned int height = image.height;
            unsigned int totalPixels = width*height;

            unsigned char* imagePtr = reinterpret_cast<unsigned char*>(image.image.data());
            int imageStride = image.component;

            std::vector<unsigned char> specular;
            std::vector<unsigned char> glossy;

            for(int i = 0; i < totalPixels; i++){
                // weird.
                specular.push_back(0xff - imagePtr[1]);
                specular.push_back(0xff - imagePtr[1]);
                specular.push_back(0xff - imagePtr[1]);
                // specular.push_back(0);
                // specular.push_back(0);
                // specular.push_back(0);
                
                glossy.push_back(imagePtr[0]);

                imagePtr += imageStride;
            }

            currentMat.push_back(Texture(reinterpret_cast<unsigned char*>(specular.data()), width, height, 3, TextureType::Specular));
            currentMat.push_back(Texture(reinterpret_cast<unsigned char*>(glossy.data()), width, height, 1, TextureType::Glossy));
        }else{
            currentMat.push_back(Texture::defaultSpecular());
            currentMat.push_back(Texture::defaultGlossy());
        }

        int normalTexIndex = material.normalTexture.index; // Get the normal texture index
        if (normalTexIndex >= 0) {
            tinygltf::Image &image = model.images[model.textures[normalTexIndex].source];

            assert(image.bits == 8);
            assert(image.component == 4); // Normal maps are usually stored as RGBA

            unsigned char* imagePtr = reinterpret_cast<unsigned char*>(image.image.data());
            currentMat.push_back(Texture(imagePtr, image.width, image.height, 4, TextureType::Normal));
        }else{
            currentMat.push_back(Texture::defaultNormal());
        }


        materials.push_back(currentMat);

        // int normalTexIndex = material.normalTexture.index;
        // int occlusionTexIndex = material.occlusionTexture.index;
        // int emissiveTexIndex = material.emissiveTexture.index;
    }


    Gameobject* rootGameobject = new Gameobject(path);
    std::vector<int> sceneNodes = model.scenes[0].nodes;
    for(const int rootNode : sceneNodes){
        Gameobject* gameobject = processNode(rootNode, glm::mat4(1.0), model, materials);
        gameobject->setParent(rootGameobject);
    }

    return rootGameobject;
}

// NORMAL
// POSITION
// TANGENT
// TEXCOORD_0
