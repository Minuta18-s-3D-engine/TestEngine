#include "MeshGen.hpp"

// Source: https://pastebin.com/DXKEmvap

std::shared_ptr<Mesh> generateCubeMesh(
    glm::vec3 scale, glm::vec2 textureScale,
    std::string textureKey, AssetManager& manager, Material mat
) {
    float data[24 * 14] = {
        // POS         NORMAL    TEX CORDS      TAN      BITAN
        -1, -1,  1, -1,  0,  0,    0,  0,    0, 1,  0,  0,  0, -1, 
        -1,  1,  1, -1,  0,  0,    0,  1,    0,  1,  0,  0,  0, -1, 
        -1,  1, -1, -1,  0,  0,    1,  1,    0,  1,  0,  0,  0, -1, 
        -1, -1, -1, -1,  0,  0,    1,  0,    0,  1,  0,  0,  0, -1, 
        -1, -1, -1,  0,  0, -1,    0,  0,    0,  1,  0,  1,  0,  0, 
        -1,  1, -1,  0,  0, -1,    0,  1,    0,  1,  0,  1,  0,  0, 
         1,  1, -1,  0,  0, -1,    1,  1,    0,  1,  0,  1,  0,  0, 
         1, -1, -1,  0,  0, -1,    1,  0,    0,  1,  0,  1,  0,  0, 
         1, -1, -1,  1,  0,  0,    0,  0,    0,  1,  0,  0,  0,  1, 
         1,  1, -1,  1,  0,  0,    0,  1,    0,  1,  0,  0,  0,  1, 
         1,  1,  1,  1,  0,  0,    1,  1,    0,  1,  0,  0,  0,  1, 
         1, -1,  1,  1,  0,  0,    1,  0,    0,  1,  0,  0,  0,  1, 
         1, -1,  1,  0,  0,  1,    0,  0,    0,  1,  0, -1,  0,  0, 
         1,  1,  1,  0,  0,  1,    0,  1,    0,  1,  0, -1,  0,  0, 
        -1,  1,  1,  0,  0,  1,    1,  1,    0,  1,  0, -1,  0,  0, 
        -1, -1,  1,  0,  0,  1,    1,  0,    0,  1,  0, -1,  0,  0, 
        -1, -1, -1,  0, -1,  0,    0,  0,    1,  0,  0,  0,  0,  1, 
         1, -1, -1,  0, -1,  0,    0,  1,    1,  0,  0,  0,  0,  1, 
         1, -1,  1,  0, -1,  0,    1,  1,    1,  0,  0,  0,  0,  1, 
        -1, -1,  1,  0, -1,  0,    1,  0,    1,  0,  0,  0,  0,  1, 
         1,  1, -1,  0,  1,  0,    0,  0,   -1,  0,  0,  0,  0,  1, 
        -1,  1, -1,  0,  1,  0,    0,  1,   -1,  0,  0,  0,  0,  1, 
        -1,  1,  1,  0,  1,  0,    1,  1,   -1,  0,  0,  0,  0,  1, 
         1,  1,  1,  0,  1,  0,    1,  0,   -1,  0,  0,  0,  0,  1
    };
    
    uint indices[6 * 6] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7, 
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    int texInds[6] = { 0, 1, 3, 3, 1, 2 };

    std::vector<Vertex> cubeVertices;
    for (int i = 0; i < 36; ++i) {
        Vertex v;
        v.pos = glm::vec3(
            data[i * 14] * scale.x, 
            data[i * 14 + 1] * scale.y, 
            data[i * 14 + 2] * scale.z
        );
        v.normal = glm::vec3(
            data[i * 14 + 3], 
            data[i * 14 + 4], 
            data[i * 14 + 5]
        );
        v.texCords = glm::vec2(
            data[i * 14 + 6] * textureScale.x, 
            data[i * 14 + 7] * textureScale.y
        );
        v.tangent = glm::vec3(
            data[i * 14 + 8], 
            data[i * 14 + 9], 
            data[i * 14 + 10]
        );
        v.bitangent = glm::vec3(
            data[i * 14 + 11], 
            data[i * 14 + 12], 
            data[i * 14 + 13]
        );
        cubeVertices.push_back(v);
    }

    std::vector<uint> cubeIndices(36);
    for (int i = 0; i < 36; ++i) {
        cubeIndices[i] = indices[i];
    }

    std::vector<TextureMaterial> cubeTextures = { 
        manager.require<TextureMaterial>(textureKey), 
        manager.require<TextureMaterial>(textureKey + "Specular")
    };

    return std::make_shared<Mesh>(
        cubeVertices, cubeIndices, cubeTextures, mat);
}
