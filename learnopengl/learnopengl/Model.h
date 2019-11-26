#pragma once

#include "Object.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Texture.h"

using namespace glm;
using namespace Assimp;


namespace yk
{
	class Model : public Object
	{
		TYPE(yk::Model, Object);

	public:
		Model(string path):Object()
		{
			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | 
				aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
				return;
			}

			directory = path.substr(0, path.find_last_of('/'));
			processNode(scene->mRootNode, scene);

		}
		void draw(Shader shader) {
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				meshes[i].draw(shader);
			}
		}

		Mesh getMesh(int i=0)
		{
			return meshes[i];
		}
		vector<Mesh> getMeshes()
		{
			return meshes;
		}

		static Model Actor()
		{
			auto model = new Model("model/nanosuit.obj");
			return *model;
		}

	private:
		vector<Mesh> meshes;
		string directory;
		vector<Texture> textures_loaded;
		void processNode(aiNode *node, const aiScene *scene)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				auto mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}
		}
		Mesh processMesh(aiMesh *mesh, const aiScene *scene) 
		{
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				vec3 vert;
				vert.x = mesh->mVertices[i].x;
				vert.y = mesh->mVertices[i].y;
				vert.z = mesh->mVertices[i].z;
				vertex.position = vert;

				vert.x = mesh->mNormals[i].x;
				vert.y = mesh->mNormals[i].y;
				vert.z = mesh->mNormals[i].z;
				vertex.normal = vert;
				
				if (mesh->mTextureCoords[0])
				{
					vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoord = vec;
				}
				else
					vertex.texCoord = vec2(0, 0);

				/*vert.x = mesh->mTangents[i].x;
				vert.y = mesh->mTangents[i].y;
				vert.z = mesh->mTangents[i].z;
				vertex.tangent = vert;*/

				/*vert.x = mesh->mBitangents[i].x;
				vert.y = mesh->mBitangents[i].y;
				vert.z = mesh->mBitangents[i].z;
				vertex.bitangent = vert;*/

				vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}

			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
				vector<Texture> diffuseMaps = loadMaterialTextures(material,
					aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

				vector<Texture> specularMaps = loadMaterialTextures(material,
					aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

				vector<Texture> normalMaps = loadMaterialTextures(material,
					aiTextureType_HEIGHT, "texture_normal");
				textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

				std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
				textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}
		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string tpname)
		{
			vector<Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
					if ((textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
					{
						textures.push_back(textures_loaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip)
				{
					Texture texture;
					texture.id = TextureLoader::TextureFromFile(str.C_Str(), directory);
					texture.type = tpname;
					texture.path = str.C_Str();
					textures.push_back(texture);
					textures_loaded.push_back(texture);
				}
				
			}
			return textures;
		}
	
	public:
		static shared_ptr<Mesh> box()
		{
			vector<float> posVec = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};

			vector<Vertex> verts;
			vector<unsigned int> indices;
			for (size_t i = 0; i < posVec.size() / 5; i++)
			{
				Vertex v;
				v.position.x = posVec[i];
				v.position.y = posVec[i + 1];
				v.position.z = posVec[i + 2];
				v.texCoord.x = posVec[i + 3];
				v.texCoord.y = posVec[i + 4];
				verts.push_back(v);
				indices.push_back(i);
			}

			TextureLoader ctex;
			ctex.load("container.jpg", GL_RGB);
			Texture tex;
			tex.id = ctex.getTextureID();
			tex.type = "texture_diffuse";
			tex.path = "";

			return make_shared<Mesh>(verts, indices, vector<Texture>({ tex }));
		}
};
}