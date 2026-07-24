#ifndef ENGINE_MODELS_MESH_H_
#define ENGINE_MODELS_MESH_H_

#include "glm/glm.hpp"
#include <vector>
#include "../utils/EngineTypes.h"
#include "../graphics/Shader.hpp"
#include "../materials/Material.hpp"
#include "../materials/MaterialInstance.hpp"
#include "engine/resource/ResourceHandle.hpp"
#include "engine/resource/ResourceManager.hpp"

struct Vertex {
    glm::vec3 pos, normal;
    glm::vec2 texCords;
    glm::vec3 tangent, bitangent;
};

class Mesh {
    uint VAO = 0, VBO = 0, EBO = 0;

    void setupMesh();
public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    ResourceHandle<MaterialInstance> materialInstanceHandle;

    ResourceManager* resourceManager;

    Mesh(
        std::vector<Vertex> vertices_,
        std::vector<uint> indices_,
        ResourceHandle<MaterialInstance> materialInstanceHandle_,
        ResourceManager& resourceManager_
    );
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void draw() const;
};

#endif // ENGINE_MODELS_MESH_H_
