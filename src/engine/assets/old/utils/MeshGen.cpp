#include "MeshGen.hpp"

// Source: https://pastebin.com/DXKEmvap

std::shared_ptr<Mesh> generateCubeMesh(
    glm::vec3 scale, glm::vec2 textureScale,
    std::shared_ptr<MaterialInstance> mat
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
    for (int i = 0; i < 24; ++i) {
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

    return std::make_shared<Mesh>(
        std::move(cubeVertices), 
        std::move(cubeIndices), 
        mat
    );
}

// Source: 

std::shared_ptr<Mesh> generateSphereMesh(
    float radius, uint32_t sectors, uint32_t stacks,
    std::shared_ptr<MaterialInstance> material
) {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    const float PI = std::acos(-1.0f);
    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    float sectorAngle, stackAngle;

    for (uint32_t i = 0; i <= stacks; ++i) {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * std::cos(stackAngle);
        z = radius * std::sin(stackAngle);

        for (uint32_t j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;

            Vertex vertex;
            x = xy * std::cos(sectorAngle);
            y = xy * std::sin(sectorAngle);
            vertex.pos = glm::vec3(x, y, z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertex.normal = glm::vec3(nx, ny, nz);

            s = (float) j / sectors;
            t = (float) i / stacks;
            vertex.texCords = glm::vec2(s, t);

            vertex.tangent = glm::vec3(
                -std::sin(sectorAngle), std::cos(sectorAngle), 0.0f
            );
            vertex.bitangent = glm::cross(vertex.normal, vertex.tangent);

            vertices.push_back(vertex);
        }
    }

    uint32_t k1, k2;
    for (uint32_t i = 0; i < stacks; ++i) {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for (uint32_t j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_shared<Mesh>(
        std::move(vertices), std::move(indices), material
    );
}
