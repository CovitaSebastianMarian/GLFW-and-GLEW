#pragma once
#ifdef SEBAPI
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace seb {

    class Mesh : public Object3D {
    public:
        std::vector<float> position_coords;
        std::vector<float> texture_coords;
        std::vector<float> normal_coords;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        glm::vec3 materialColor = glm::vec3(1.0f, 1.0f, 1.0f);

        void create() {
            vbo_data.resize(3);

            vbo_data[0] = position_coords;
            vbo_data[1] = texture_coords;
            vbo_data[2] = normal_coords;
            ebo_data = indices;

            nondsa_generate_buffers(3);
            nondsa_initialize_vertex_buffer(0, 3, GL_STATIC_DRAW);
            nondsa_initialize_vertex_buffer(1, 2, GL_STATIC_DRAW);
            nondsa_initialize_vertex_buffer(2, 3, GL_STATIC_DRAW);
            nondsa_initialize_elements_buffer(GL_STATIC_DRAW);
        }
    };

    class Model {
    public:
        std::vector<Mesh> meshes;
        std::string directory;

        void load(const std::string path) {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path,
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));

            processNode(scene->mRootNode, scene);
        }
    private:

        void processNode(aiNode* node, const aiScene* scene) {
            for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
            Mesh m;
            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                m.position_coords.push_back(mesh->mVertices[i].x);
                m.position_coords.push_back(mesh->mVertices[i].y);
                m.position_coords.push_back(mesh->mVertices[i].z);

                if (mesh->HasNormals()) {
                    m.normal_coords.push_back(mesh->mNormals[i].x);
                    m.normal_coords.push_back(mesh->mNormals[i].y);
                    m.normal_coords.push_back(mesh->mNormals[i].z);
                }
                else {
                    m.normal_coords.push_back(0.0f);
                    m.normal_coords.push_back(0.0f);
                    m.normal_coords.push_back(0.0f);
                }

                if (mesh->mTextureCoords[0]) {
                    m.texture_coords.push_back(mesh->mTextureCoords[0][i].x);
                    m.texture_coords.push_back(mesh->mTextureCoords[0][i].y);
                }
                else {
                    m.texture_coords.push_back(0.0f);
                    m.texture_coords.push_back(0.0f);
                }
            }


            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    m.indices.push_back(face.mIndices[j]);
                }
            }

            if (mesh->mMaterialIndex >= 0) {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

                std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                if (!diffuseMaps.empty()) {
                    m.textures = diffuseMaps;
                }
                else {
                    aiColor3D color(1.0f, 1.0f, 1.0f);
                    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                    m.materialColor = glm::vec3(color.r, color.g, color.b);
                }
            }

            m.create();
            return m;
        }
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
            std::vector<Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
                aiString str;
                mat->GetTexture(type, i, &str);
                std::string filename = directory + "/" + std::string(str.C_Str());
                Texture texture;
                texture.type = typeName;
                texture.path = filename;
                std::cout<<filename<<std::endl;

                std::string extension = filename.substr(filename.find_last_of('.') + 1);
                int internalFormat;
                if (extension == "png") {
                    internalFormat = GL_RGBA;
                }
                else if (extension == "jpg") {
                    internalFormat = GL_RGB;
                }
                else {
                    internalFormat = GL_RGB;
                }

                texture.open(filename.c_str(), internalFormat);
                textures.push_back(texture);
            }
            return textures;
        }
    };
}
#endif