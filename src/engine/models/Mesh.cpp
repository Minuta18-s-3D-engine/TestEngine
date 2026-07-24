#include "Mesh.hpp"

Mesh::Mesh(
    std::vector<Vertex> vertices_,
    std::vector<uint> indices_,
    const ResourceHandle<MaterialInstance> materialInstanceHandle_,
    ResourceManager& resourceManager_
) : vertices(std::move(vertices_)), indices(std::move(indices_)),
    materialInstanceHandle(materialInstanceHandle_),
    resourceManager(&resourceManager_) {
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(Mesh&& other) noexcept
  : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
    vertices(std::move(other.vertices)),
    indices(std::move(other.indices)),
    materialInstanceHandle(other.materialInstanceHandle),
    resourceManager(other.resourceManager) {
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.resourceManager = nullptr;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        materialInstanceHandle = other.materialInstanceHandle;
        resourceManager = other.resourceManager;

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.resourceManager = nullptr;
    }
    return *this;
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
        &vertices[0],
        GL_STATIC_DRAW
    );
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(uint)),
        &indices[0],
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        reinterpret_cast<void*>(offsetof(Vertex, normal)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        reinterpret_cast<void*>(offsetof(Vertex, texCords)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        reinterpret_cast<void*>(offsetof(Vertex, tangent)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw() const {
    const auto& materialInstance = resourceManager->require<MaterialInstance>(
        materialInstanceHandle);
    const auto shaderHandle = materialInstance.getMaterial().getShader();
    auto& shader = resourceManager->require<Shader>(shaderHandle);

    shader.setUniform(
        "u_CurrentMaterialStartId",
        materialInstance.getProperties().getStartId() / 4
    );
    materialInstance.bindSamplers();

    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT,
        0
    );
    glBindVertexArray(0);
}
