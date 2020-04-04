#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <skinnedTexture.hpp>
#include <map>
#include <vector>
#include <string>

#include "Component.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
class SkinnedMesh : public  Component{
public:
    SkinnedMesh();

    ~SkinnedMesh();

    bool LoadMesh(const std::string &Filename);

    void Render(Camera &camera, float time);

    unsigned int NumBones() const {
        return m_NumBones;
    }

    void BoneTransform(float TimeInSeconds, std::vector<glm::mat4> &Transforms);

private:
    static const unsigned int NUM_BONES_PER_VERTEX = 4;

    struct BoneInfo {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;

        BoneInfo() {
            BoneOffset = glm::mat4(0);
            FinalTransformation = glm::mat4(0);
        }
    };

    struct VertexBoneData {
        unsigned int IDs[NUM_BONES_PER_VERTEX];
        float Weights[NUM_BONES_PER_VERTEX];

        VertexBoneData() {
            Reset();
        };

        void Reset() {
            for (auto &i:IDs)
                i = 0;
            for (auto &i:Weights)
                i = 0;
        }

        void AddBoneData(unsigned int BoneID, float Weight);
    };

    static const unsigned int INVALID_MATERIAL = 0xFFFFFFFF;
    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs
    };
    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry() {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        std::vector<SkinnedTexture> m_Textures;
    };

    bool InitFromScene(const aiScene *pScene, const std::string &Filename);

    void InitMesh(unsigned int Index, const aiMesh *paiMesh, std::vector<glm::vec3> &Positions, std::vector<glm::vec3> &Normals,
                  std::vector<glm::vec2> &TexCoords, std::vector<VertexBoneData> &Bones, std::vector<unsigned int> &Indices);

    void InitMeshMaterial(const aiMaterial *pMaterial, std::vector<SkinnedTexture> &mesh_textures);

    SkinnedTexture LoadMaterialFile(const aiMaterial *pMaterial, aiTextureType aiType, TextureEnum texEnum);

    void LoadBones(unsigned int MeshIndex, const aiMesh *pMesh, std::vector<VertexBoneData> &Bones);

    void ReadNodeHierarchy(float AnimationTime, const aiNode *pNode, const glm::mat4 &ParentTransform);

    const aiNodeAnim *FindNodeAnim(const aiAnimation *pAnimation, const std::string &NodeName);

    void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    void CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    unsigned int FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim);

    unsigned int FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim);

    unsigned int FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim);

    static glm::mat4 convertToMat4(const aiMatrix4x4 &m);

    static glm::mat4 convertToMat4(const aiMatrix3x3 &m);

    void Clear();

    std::string Dir;
    std::vector<MeshEntry> m_Entries;
    std::vector<SkinnedTexture> m_Textures_Loaded;

    std::map<std::string, unsigned int> m_BoneMapping;
    unsigned int m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;
    glm::mat4 m_GlobalInverseTransform;

    const aiScene *m_pScene;
    Assimp::Importer m_Importer;

    Shader shader;
};



