#include "ModelLoader.hpp"

// learnopengl.com saves the day once again
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h

std::unique_ptr<Model> ModelLoader::loadModel(
    const VirtualPath& filename,
    ResourceHandle<Material> baseMaterial,
    ResourceManager& resManager
) {
    this->baseMaterialHandle = baseMaterial;
    this->resourceManager = &resManager;

    std::string resolvedPath = filename.resolve();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        resolvedPath,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(
            std::string("Assimp error: ") + importer.GetErrorString()
        );
    }

    directory = resolvedPath.substr(0, resolvedPath.find_last_of('/'));

    createdModel = std::make_unique<Model>(*resourceManager);
    createdModel->material = resourceManager->getByPath<Material>("materials/standardMaterial");

    loadedTextures.clear();
    loadedMaterials.clear();

    processNode(scene->mRootNode, scene);

    return std::move(createdModel);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        createdModel->meshes.push_back(processMesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

ResourceHandle<Mesh> ModelLoader::processMesh(
    aiMesh* mesh, const aiScene* scene
) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex{};
        vertex.pos = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        if (mesh->HasNormals()) {
            vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }

        if (mesh->mTextureCoords[0]) {
            vertex.texCords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
            vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
        } else {
            vertex.texCords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    ResourceHandle<MaterialInstance> meshMaterialHandle;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        meshMaterialHandle = loadMaterial(material, mesh->mMaterialIndex, scene);
    } else {
        const Material& baseMat = resourceManager->require(baseMaterialHandle);
        MaterialInstance instance(
            baseMat.getName() + "Instance",
            baseMat,
            baseMat.getDefaultValues().getBuffer(),
            *resourceManager
        );
        meshMaterialHandle = resourceManager->addManually(std::move(instance));
    }

    Mesh rawMesh(
        std::move(vertices),
        std::move(indices),
        meshMaterialHandle,
        *resourceManager
    );

    return resourceManager->addManually(std::move(rawMesh));
}

ResourceHandle<MaterialInstance> ModelLoader::loadMaterial(
    aiMaterial* mat,
    uint32_t matIndex,
    const aiScene* scene
) {
    if (loadedMaterials.contains(matIndex)) {
        return loadedMaterials[matIndex];
    }

    const Material& baseMat = resourceManager->require(baseMaterialHandle);

    MaterialInstance instance(
        baseMat.getName() + "Instance",
        baseMat,
        baseMat.getDefaultValues().getBuffer(),
        *resourceManager
    );

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        std::string path = str.C_Str();

        ResourceHandle<Texture> tex;
        if (path[0] == '*') {
            int texIndex = std::stoi(path.substr(1));
            tex = loadEmbeddedTexture(scene->mTextures[texIndex], path);
        } else {
            tex = loadExternalTexture(path);
        }

        if (tex.isValid()) {
            instance.setSampler("diffuseMap", tex);
        }
    }

    if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString str;
        mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
        std::string path = str.C_Str();

        ResourceHandle<Texture> tex;
        if (path[0] == '*') {
            int texIndex = std::stoi(path.substr(1));
            tex = loadEmbeddedTexture(scene->mTextures[texIndex], path);
        } else {
            tex = loadExternalTexture(path);
        }

        if (tex.isValid()) {
            instance.setSampler("specularMap", tex);
        }
    }

    ResourceHandle<MaterialInstance> instanceHandle = resourceManager->addManually(std::move(instance));
    loadedMaterials[matIndex] = instanceHandle;
    return instanceHandle;
}

ResourceHandle<Texture> ModelLoader::loadExternalTexture(
    const std::string& path
) {
    if (loadedTextures.contains(path)) {
        return loadedTextures[path];
    }

    size_t len = 0;
    auto fullPath = directory + "/" + path;
    auto textureContent = read_bytes(fullPath, len);

    ResourceHandle<Texture> textureHandle = this->loadTexture(
        textureContent.get(), len, fullPath
    );

    loadedTextures[path] = textureHandle;
    return textureHandle;
}

ResourceHandle<Texture> ModelLoader::loadEmbeddedTexture(
    const aiTexture* embeddedTexture,
    const std::string& embeddedId
) {
    if (loadedTextures.contains(embeddedId)) {
        return loadedTextures[embeddedId];
    }

    if (embeddedTexture->mHeight == 0) {
        ResourceHandle<Texture> textureHandle = this->loadTexture(
            reinterpret_cast<const uint8_t*>(embeddedTexture->pcData),
            embeddedTexture->mWidth,
            embeddedId
        );

        loadedTextures[embeddedId] = textureHandle;
        return textureHandle;
    } else {
        throw std::runtime_error(
            "Uncompressed textures are not implemented yet"
        );
    }
}

ResourceHandle<Texture> ModelLoader::loadTexture(
    const uint8_t* imgData,
    size_t imgSize,
    const std::string& name
) {
    ImageType imgFormat = getImageType(imgData, imgSize);
    std::shared_ptr<ImageData> rawTexture = nullptr;

    if (imgFormat == ImageType::PNG) {
        rawTexture = PngCoder::load_image(imgData, imgSize, name);
    } else if (imgFormat == ImageType::JPG) {
        rawTexture = JpgCoder::load_image(imgData, imgSize, name);
    } else {
        throw std::runtime_error("Unsupported image type");
    }

    if (!rawTexture) {
        return ResourceHandle<Texture>::createNullHandle();
    }

    Texture texture(
        *rawTexture, TextureSchema(), SamplerType::Texture2D
    );

    return resourceManager->addManually(
        VirtualPath(name), std::move(texture)
    );
}
