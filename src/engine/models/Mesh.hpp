#ifndef ENGINE_MODELS_MESH_H_
#define ENGINE_MODELS_MESH_H_

#include "glm/glm.hpp"
#include <vector>
#include "../utils/EngineTypes.h"
#include "../materials/TextureMaterial.hpp"
#include "../materials/Material.hpp"
#include "../graphics/Shader.hpp"

struct Vertex {
    glm::vec3 pos, normal;
    glm::vec2 texCords;
    glm::vec3 tangent, bitangent;
};

class Mesh {
    uint VAO, VBO, EBO;

    void setupMesh();
public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<TextureMaterial> textures;
    Material mainMaterial;
    bool hasMaterial;

    Mesh(std::vector<Vertex> _vertices, std::vector<uint> _indices, 
        std::vector<TextureMaterial> _textures, Material _mainMaterial);
    Mesh(std::vector<Vertex> _vertices, std::vector<uint> _indices, 
        std::vector<TextureMaterial> _textures);
    ~Mesh();
    
    void draw(Shader& shader);
};

#endif // ENGINE_MODELS_MESH_H_
