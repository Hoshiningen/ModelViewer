#include "IO/GeometryLoader.hpp"

#include "Geometry/VertexBuffer.hpp"

#include <iostream>
#include <stack>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct GeometryLoader::Private {

    std::forward_list<VertexBuffered> importGeometry(aiNode* pNode, const aiScene* pScene) const;
    VertexBuffered processMesh(aiMesh* pMesh, const aiScene* pScene) const;
};

std::forward_list<VertexBuffered> GeometryLoader::Private::importGeometry(aiNode* pRoot, const aiScene* pScene) const {

    std::forward_list<VertexBuffered> meshes;

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
            meshes.push_front(processMesh(pMesh, pScene));
        }

        // Add all children so we can process their meshes too.
        for (unsigned int childIndex = 0; childIndex < pNode->mNumChildren; ++childIndex)
            nodes.push(pNode->mChildren[childIndex]);
    }

    return meshes;
}

VertexBuffered GeometryLoader::Private::processMesh(aiMesh* pMesh, const aiScene* pScene) const {

    VertexBuffer buffer;

    for (unsigned int vertIndex = 0; vertIndex < pMesh->mNumVertices; ++vertIndex) {

        if (pMesh->HasPositions())
            buffer.addVertex({ pMesh->mVertices[vertIndex].x, pMesh->mVertices[vertIndex].y , pMesh->mVertices[vertIndex].z });

        if (pMesh->HasNormals())
            buffer.addNormal({ pMesh->mNormals[vertIndex].x, pMesh->mNormals[vertIndex].y, pMesh->mNormals[vertIndex].z });
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


GeometryLoader::GeometryLoader()
    : m_pPrivate(std::make_unique<Private>()) {}

GeometryLoader::~GeometryLoader() {}

GeometryLoader::GeometryLoader(const GeometryLoader& other) {
    *this = other;
}

GeometryLoader& GeometryLoader::operator=(const GeometryLoader& other) {

    if (this != &other)
        m_pPrivate = std::make_unique<Private>(*other.m_pPrivate);

    return *this;
}

GeometryLoader::GeometryLoader(GeometryLoader&& other) noexcept {
    *this = std::move(other);
}

GeometryLoader& GeometryLoader::operator=(GeometryLoader&& other) noexcept {

    if (this != &other)
        m_pPrivate = std::exchange(other.m_pPrivate, nullptr);

    return *this;
}

std::forward_list<VertexBuffered> GeometryLoader::loadGeometry(const std::filesystem::path& path) const {

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

    return m_pPrivate->importGeometry(pScene->mRootNode, pScene);
}
