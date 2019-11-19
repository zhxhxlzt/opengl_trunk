#pragma once

#include "Object.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.hpp"

using namespace glm;
using namespace Assimp;

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 tangent;
	vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	string type;
	aiString path;
};
namespace yk
{
	class Mesh : public Object
	{
		TYPE(yk::Mesh, yk::Object)
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) :
			vertices(vertices), indices(indices), textures(textures)
		{
			setupMesh();
		}
		void draw(Shader shader)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			//for (unsigned int i = 0; i < textures.size(); i++)
			//{
			//	glActiveTexture(GL_TEXTURE0 + i);
			//	string number;
			//	string name = textures[i].type;
			//	if (name == "texture_diffuse")
			//		number = to_string(diffuseNr++);
			//	else if (name == "texture_specular")
			//		number = to_string(specularNr++);

			//	shader.set((/*"material." + */name + number).c_str(), i);
			//	glActiveTexture(GL_TEXTURE0 + i);
			//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
			//}
			//glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		unsigned int m_vao, m_vbo, m_ebo;
		void setupMesh()
		{
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
			glGenBuffers(1, &m_ebo);

			glBindVertexArray(m_vao);
			
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),&vertices[0], 
				GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
				&indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

			glBindVertexArray(0);
		}
	};

	class Model : public Object
	{
		TYPE(yk::Model, yk::Object);

	public:
		Model(string path)
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
					texture.id = TextureFromFile(str.C_Str(), directory);
					texture.type = tpname;
					texture.path = str.C_Str();
					textures.push_back(texture);
					textures_loaded.push_back(texture);
				}
				
			}
			return textures;
		}

		static unsigned int TextureFromFile(const char *path, const string &directory)
		{
			string filename = string(path);
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return textureID;
		}
	};
}