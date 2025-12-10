#include "ModelLoader.hpp"

// learnopengl.com saves the day once again
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h

std::unique_ptr<Model> ModelLoader::loadModel(const std::string& filename) {
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

    processNode(scene->mRootNode, scene);

    return std::move(createdModel);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
    for (uint i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        createdModel->meshes.push_back(
            std::make_shared<Mesh>(processMesh(mesh, scene))
        );
    }

    for (uint i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<TextureMaterial> textures;

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

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<TextureMaterial> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<TextureMaterial> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular", scene);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<TextureMaterial> normalMaps = loadMaterialTextures(
        material, aiTextureType_HEIGHT, "texture_normal", scene);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<TextureMaterial> heightMaps = loadMaterialTextures(
        material, aiTextureType_AMBIENT, "texture_height", scene);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<TextureMaterial> ModelLoader::loadMaterialTextures(
    aiMaterial* mat, 
    aiTextureType type, 
    const std::string& typeName,
    const aiScene* scene
) {
    std::vector<TextureMaterial> textures;
    for (uint i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (uint j = 0; j < loadedTextures.size(); ++j) {
            if (std::strcmp(
                loadedTextures[j].filename.data(),
                str.C_Str()
            ) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }        

        if (!skip) {
            TextureType texType;
            if (typeName == "texture_diffuse") {
                texType = TextureType::DIFFUSE;
            } else if (typeName == "texture_specular") {
                texType = TextureType::SPECULAR;
            } else if (typeName == "texture_normal") {
                texType = TextureType::NORMAL;
            } else if (typeName == "texture_height") {
                texType = TextureType::HEIGHT;
            }

            TextureMaterial textureMat;

            std::string texturePath = str.C_Str();
            // IMPORTANT: Embedded textures are marked "*" in assimp
            if (texturePath[0] == '*') {
                int textureIndex = std::stoi(texturePath.substr(1));
                if (textureIndex >= 0 && 
                    textureIndex < (int) scene->mNumTextures) {
                    textureMat = loadEmbeddedTexture(
                        scene->mTextures[textureIndex],
                        texType,
                        texturePath
                    );
                }
            } else {
                textureMat = loadExternalTexture(texturePath, texType);
            }

            textures.push_back(textureMat);
            loadedTextures.push_back(textureMat);
        }
    }

    return textures;
}

TextureMaterial ModelLoader::loadExternalTexture(
    const std::string& path, TextureType type
) {    
    size_t len = 0;
    auto fullPath = directory + "/" + path;
    auto textureContent = read_bytes(fullPath, len);
    auto texture = PngCoder::load_texture(
        textureContent.get(), len, fullPath);

    auto textureMat = TextureMaterial(texture, type);
        textureMat.filename = fullPath;

    return textureMat;
}

TextureMaterial ModelLoader::loadEmbeddedTexture(
    const aiTexture* embeddedTexture, TextureType texType,
    const std::string& embeddedId
) {
    if (embeddedTexture->mHeight == 0) {
        auto texture = PngCoder::load_texture(
            reinterpret_cast<const uint8_t*>(embeddedTexture->pcData),
            embeddedTexture->mWidth,
            embeddedId
        );

        auto res = TextureMaterial(texture, texType);
        res.filename = embeddedId;

        return res;
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
