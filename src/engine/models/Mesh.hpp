#ifndef ENGINE_MODELS_MESH_H_
#define ENGINE_MODELS_MESH_H_

#include "glm/glm.hpp"
#include <vector>
#include "../utils/EngineTypes.h"
#include "../graphics/Shader.hpp"
#include "../materials/Material.hpp"
#include "../materials/MaterialInstance.hpp"

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
    std::shared_ptr<MaterialInstance> material;

    Mesh(std::vector<Vertex> _vertices, std::vector<uint> _indices, 
        std::shared_ptr<MaterialInstance> _material);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    
    void draw(Shader& shader);
};

#endif // ENGINE_MODELS_MESH_H_
