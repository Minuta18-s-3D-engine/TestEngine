#include "ModelLoader.hpp"

// learnopengl.com saves the day once again
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h

std::unique_ptr<Model> ModelLoader::loadModel(
    const VirtualPath& _filename, 
    std::shared_ptr<Material> _baseMaterial,
    const AssetManager& _assetManager
) {
    baseMaterial = _baseMaterial;

    std::string filename = _filename.resolve();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filename, 
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace 
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
        !scene->mRootNode) {
        throw std::runtime_error(
            std::string("Assimp error: ") + importer.GetErrorString());
    }

    directory = filename.substr(0, filename.find_last_of('/'));
    createdModel = std::make_unique<Model>();
    createdModel->material = _assetManager.getShared<Material>("materials/standardMaterial"); 

    loadedTextures.clear();
    loadedMaterials.clear();

    processNode(scene->mRootNode, scene);

    return std::move(createdModel);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
    for (uint i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        createdModel->meshes.push_back(processMesh(mesh, scene));
    }

    for (uint i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> ModelLoader::processMesh(
    aiMesh* mesh, const aiScene* scene
) {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;

    for (uint i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        glm::vec3 tempVector;
        tempVector.x = mesh->mVertices[i].x;
        tempVector.y = mesh->mVertices[i].y;
        tempVector.z = mesh->mVertices[i].z;
        vertex.pos = tempVector;

        if (mesh->HasNormals()) {
            tempVector.x = mesh->mNormals[i].x;
            tempVector.y = mesh->mNormals[i].y;
            tempVector.z = mesh->mNormals[i].z;
            vertex.normal = tempVector;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 tempVector2;
            tempVector2.x = mesh->mTextureCoords[0][i].x;
            tempVector2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCords = tempVector2;

            tempVector.x = mesh->mTangents[i].x;
            tempVector.y = mesh->mTangents[i].y;
            tempVector.z = mesh->mTangents[i].z;
            vertex.tangent = tempVector;

            tempVector.x = mesh->mBitangents[i].x;
            tempVector.y = mesh->mBitangents[i].y;
            tempVector.z = mesh->mBitangents[i].z;
            vertex.bitangent = tempVector;
        } else {
            vertex.texCords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::shared_ptr<MaterialInstance> meshMaterial = nullptr;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        meshMaterial = loadMaterial(material, mesh->mMaterialIndex, scene);
    } else {
        meshMaterial = std::make_shared<MaterialInstance>(
            baseMaterial->getName() + "Instance", 
            *baseMaterial,
            baseMaterial->getDefaultValues().getBuffer()
        );
    }

    return std::make_shared<Mesh>(
        std::move(vertices), 
        std::move(indices), 
        meshMaterial
    );
}

std::shared_ptr<MaterialInstance> ModelLoader::loadMaterial(
    aiMaterial* mat, 
    uint32_t matIndex,
    const aiScene* scene
) {
    if (loadedMaterials.contains(matIndex)) {
        return loadedMaterials[matIndex];
    }

    auto instance = std::make_shared<MaterialInstance>(
        baseMaterial->getName() + "Instance", 
        *baseMaterial,
        baseMaterial->getDefaultValues().getBuffer()
    );

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        std::string path = str.C_Str();

        std::shared_ptr<Texture> tex = nullptr;
        if (path[0] == '*') {
            int texIndex = std::stoi(path.substr(1));
            tex = loadEmbeddedTexture(scene->mTextures[texIndex], path);
        } else {
            tex = loadExternalTexture(path);
        }

        if (tex) {
            instance->setSampler("diffuseMap", tex);
        }
    }

    if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString str;
        mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
        std::string path = str.C_Str();

        std::shared_ptr<Texture> tex = nullptr;
        if (path[0] == '*') {
            int texIndex = std::stoi(path.substr(1));
            tex = loadEmbeddedTexture(scene->mTextures[texIndex], path);
        } else {
            tex = loadExternalTexture(path);
        }

        if (tex) {
            instance->setSampler("specularMap", tex);
        }
    }

    loadedMaterials[matIndex] = instance;
    return instance;
}

std::shared_ptr<Texture> ModelLoader::loadExternalTexture(
    const std::string& path
) {    
    if (loadedTextures.contains(path)) {
        return loadedTextures[path];
    }

    size_t len = 0;
    auto fullPath = directory + "/" + path;
    auto textureContent = read_bytes(fullPath, len);
    auto texture = this->loadTexture(
        textureContent.get(), len, fullPath);

    loadedTextures[path] = texture;

    return texture;
}

std::shared_ptr<Texture> ModelLoader::loadEmbeddedTexture(
    const aiTexture* embeddedTexture,
    const std::string& embeddedId
) {
    if (loadedTextures.contains(embeddedId)) {
        return loadedTextures[embeddedId];
    }

    if (embeddedTexture->mHeight == 0) {
        auto texture = this->loadTexture(
            reinterpret_cast<const uint8_t*>(embeddedTexture->pcData),
            embeddedTexture->mWidth,
            embeddedId
        );

        loadedTextures[embeddedId] = texture;

        return texture;
    } else {
        throw std::runtime_error(
            "Uncompressed textures are not implemented yet"
        );
    }
}

std::shared_ptr<Texture> ModelLoader::loadTexture(
    const uint8_t* imgData,
    size_t imgSize,
    std::string name
) {
    ImageType imgFormat = getImageType(imgData, imgSize);
    if (imgFormat == ImageType::PNG) {
        return PngCoder::load_texture(
            imgData,
            imgSize,
            name
        );
    } else if (imgFormat == ImageType::JPG) {
        return JpgCoder::load_texture(
            imgData,
            imgSize,
            name
        );
    } else {
        throw std::runtime_error("Unsupported image type");
    }
}
