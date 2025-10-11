#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<uint> _indices, 
    std::vector<TextureMaterial> _textures, Material _mainMaterial) {
    vertices = _vertices;
    indices = _indices;
    textures = _textures;
    mainMaterial = _mainMaterial;

    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
        &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), 
        &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        (void*) offsetof(Vertex, normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        (void*) offsetof(Vertex, texCords));
}

void Mesh::draw(Shader& shader) {
    mainMaterial.passToShader(shader, "mainMaterial");

    uint diffuseNr = 1;
    uint specularNr = 1;
    for (uint i = 0; i < textures.size(); ++i) {
        std::string shaderName = "texture";

        if (textures[i].type == TextureType::DIFFUSE) {
            shaderName += "Diffuse" + std::to_string(diffuseNr);
            diffuseNr += 1;
        } else if (textures[i].type == TextureType::SPECULAR) {
            shaderName += "Specular" + std::to_string(specularNr);
            specularNr += 1;
        }

        textures[i].passToShader(shader, shaderName + "Mat");
        textures[i].passTextureToShader(i, shader, shaderName);
    }

    glActiveTexture(0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
