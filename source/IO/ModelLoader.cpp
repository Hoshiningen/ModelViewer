#include "IO/ModelLoader.hpp"
#include "IO/TextureLoader.hpp"

#include "Geometry/VertexBuffer.hpp"

#include <filesystem>
#include <iostream>
#include <optional>
#include <stack>
#include <sstream>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct ModelLoader::Private {

    ModelLoader::ModelProperties importMesh(aiNode* pNode, const aiScene* pScene) const;
    VertexBuffered processMesh(aiMesh* pMesh, const aiScene* pScene) const;
    void getTexturePathsByType(aiMaterial* pMaterial, aiTextureType textureType, std::unordered_multimap<Texture::Type, std::filesystem::path>& paths) const;
    Texture::Type mapAiTextureType(aiTextureType type) const;

};

ModelLoader::ModelProperties ModelLoader::Private::importMesh(aiNode* pRoot, const aiScene* pScene) const {

    ModelProperties properties;

    std::stack<aiNode*> nodes;
    std::unordered_map<aiNode*, bool> visited;

    nodes.push(pRoot);
    while (!nodes.empty()) {

        aiNode* pNode = nodes.top();
        
        if (visited[pNode])
            continue;

        visited[pNode] = true;
        nodes.pop();

        for (unsigned int meshIndex = 0; meshIndex < pNode->mNumMeshes; ++meshIndex) {

            aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[meshIndex]];
            properties.meshes.push_front(processMesh(pMesh, pScene));

            if (pMesh->mMaterialIndex >= 0) {

                for (int typeIndex = aiTextureType_NONE; typeIndex < AI_TEXTURE_TYPE_MAX; ++typeIndex) {

                    aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
                    if (!pMaterial)
                        continue;

                    getTexturePathsByType(pMaterial, static_cast<aiTextureType>(typeIndex), properties.texturePaths);
                }
            }
        }

        // Add all children so we can process their meshes too.
        for (unsigned int childIndex = 0; childIndex < pNode->mNumChildren; ++childIndex)
            nodes.push(pNode->mChildren[childIndex]);
    }

    return properties;
}

VertexBuffered ModelLoader::Private::processMesh(aiMesh* pMesh, const aiScene* pScene) const {

    VertexBuffer buffer;

    for (unsigned int vertIndex = 0; vertIndex < pMesh->mNumVertices; ++vertIndex) {

        if (pMesh->HasPositions()) {
            const aiVector3D& vertex = pMesh->mVertices[vertIndex];
            buffer.addVertex({ vertex.x, vertex.y , vertex.z });
        }

        if (pMesh->HasNormals()) {
            const aiVector3D& normal = pMesh->mNormals[vertIndex];
            buffer.addNormal({ normal.x, normal.y, normal.z });
        }

        // Only support the first set out of many.
        static constexpr unsigned int kSetIndex = 0;

        if (pMesh->HasTextureCoords(kSetIndex)) {
            const aiVector3D& texel = pMesh->mTextureCoords[kSetIndex][vertIndex];
            buffer.addTexel({ texel.x, texel.y });
        }

        if (pMesh->HasVertexColors(kSetIndex)) {
            const aiColor4D& color = pMesh->mColors[kSetIndex][vertIndex];
            buffer.addColor({ color.r, color.g, color.b, color.a });
        }
    }

    if (pMesh->HasFaces())
    {
        for (unsigned int faceIndex = 0; faceIndex < pMesh->mNumFaces; ++faceIndex)
        {
            const aiFace& face = pMesh->mFaces[faceIndex];
            for (unsigned int index = 0; index < face.mNumIndices; ++index)
                buffer.addIndex(face.mIndices[index]);
        }
    }

    return buffer;
}

void ModelLoader::Private::getTexturePathsByType(aiMaterial* pMaterial, aiTextureType textureType, std::unordered_multimap<Texture::Type, std::filesystem::path>& paths) const {
    
    if (!pMaterial)
        return;

    for (unsigned int index = 0; index < pMaterial->GetTextureCount(textureType); ++index) {
        aiString name;
        pMaterial->GetTexture(textureType, index, &name);

        if (const std::filesystem::path path = name.C_Str(); path.has_filename())
            paths.emplace(mapAiTextureType(textureType), path.filename());
    }
}

Texture::Type ModelLoader::Private::mapAiTextureType(aiTextureType type) const {
    switch (type) {
    case aiTextureType_DIFFUSE: return Texture::Type::Diffuse;
    case aiTextureType_EMISSIVE: return Texture::Type::Emissive;
    case aiTextureType_SPECULAR: return Texture::Type::Specular;
    default: return Texture::Type::Unknown;
    }
}


std::vector<std::string> ModelLoader::SupportedExtensions() {

    Assimp::Importer importer;
    
    std::string extensions;
    importer.GetExtensionList(extensions);

    std::istringstream stream{ extensions };
    std::vector<std::string> extensionList;

    std::string split;
    while (std::getline(stream, split, ';'))
        extensionList.push_back(split);

    return extensionList;
}

ModelLoader::ModelLoader()
    : m_pPrivate(std::make_unique<Private>()) {}

ModelLoader::~ModelLoader() noexcept {}

ModelLoader::ModelLoader(const ModelLoader& other) {
    *this = other;
}

ModelLoader& ModelLoader::operator=(const ModelLoader& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

ModelLoader::ModelLoader(ModelLoader&& other) noexcept {
    *this = std::move(other);
}

ModelLoader& ModelLoader::operator=(ModelLoader&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

ModelLoader::ModelProperties ModelLoader::load(const std::filesystem::path& path) const {

    constexpr unsigned int kPostProcessingFlags =
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_OptimizeGraph |
        aiProcess_OptimizeMeshes |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SplitLargeMeshes |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData;

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path.string(), kPostProcessingFlags);

    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
        std::cerr << "ASSIMP Import Error: " << importer.GetErrorString() << "\n";
        return {};
    }

    return m_pPrivate->importMesh(pScene->mRootNode, pScene);
}
