#pragma once

#include "Object.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace glm;
using namespace Assimp;
namespace yk
{
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
		Texture() = default;
		Texture(int id, string type, aiString path) :id(id), type(type), path(path) {}
	};

#define TEXTURE_CUBEMAP "texture_cubemap"
#define TEXTURE_DIFFUSE "texture_diffuse"
#define TEXTURE_SPECULAR "texture_specular"
#define TEXTURE_NORMAL "texture_normal"

	class Mesh : public Object
	{
		TYPE(yk::Mesh, Object)
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		Mesh() : vertDirty(true) {}
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) :
			vertices(vertices), indices(indices), textures(textures)
		{
			setupMesh();
		}
		void draw(Shader& shader)
		{
			if (vertDirty)
			{
				setupMesh();
			}
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int cubemapNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				string number;
				string name = textures[i].type;

				if (name == "texture_cubemap")
				{
					glCheckError();

					number = to_string(cubemapNr++);
					auto unistr = (name + number);
					
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i].id);
					shader.set(move(unistr), i);
					continue;
				}

				if (name == "texture_diffuse")
					number = to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = to_string(specularNr++);
				else if (name == "texture_normal")
					number = to_string(normalNr++);

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);

				auto unistr = (name + number);
				shader.set(move(unistr), i);
				glCheckError();
			}
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			
		}

		void CombineMeshes(vector<Mesh> meshes)
		{
			for (auto &m : meshes)
			{
				vertices.insert(vertices.end(), m.vertices.begin(), m.vertices.end());
				indices.insert(indices.end(), m.indices.begin(), m.indices.end());
				textures.insert(textures.end(), m.textures.begin(), m.textures.end());
			}
			setupMesh();
		}

		void AddVert(const Vertex &&v)
		{
			vertices.push_back(move(v));
		}

		void AddVerts(const vector<Vertex> verts)
		{
			if (verts.size() > 0)
			{
				vertices.insert(vertices.end(), verts.begin(), verts.end());
				vertDirty = true;
			}
		}
		void AddIndices(const vector<unsigned int> &indicesArray)
		{
			if (indicesArray.size() > 0)
			{
				indices.insert(indices.end(), indicesArray.begin(), indicesArray.end());
				vertDirty = true;
			}
		}
		void SetVertDirty()
		{
			vertDirty = true;
		}

	public:
		static Mesh CubeMesh()
		{
			vector<vec3> pos = {
				//front z
				vec3(-1, -1,  1),
				vec3( 1, -1,  1),
				vec3( 1,  1,  1),
				vec3(-1,  1,  1),
				//back z	
				vec3(-1, -1, -1),
				vec3( 1, -1, -1),
				vec3( 1,  1, -1),
				vec3(-1,  1, -1),
			};

			vector<float> normals = {
				
			};

			vector<vec2> texcoords = {
				// front z
				vec2(0, 0),
				vec2(1, 0),
				vec2(1, 1),
				vec2(0, 0),
				vec2(1, 1),
				vec2(0, 1)
			};

			vector<vec3> indices = {
				//front z
				vec3(0, 1, 2),
				vec3(0, 2, 3),
				//rightx
				vec3(1, 5, 6),
				vec3(1, 6, 2),
				//back z
				vec3(5, 4, 7),
				vec3(5, 7, 6),
				//left x,
				vec3(4, 0, 3),
				vec3(4, 3, 7),
				//up y	
				vec3(3, 2, 6),
				vec3(3, 6, 7),
				//down y
				vec3(0, 4, 5),
				vec3(0, 5, 1),
			};
			vector<Vertex> vertices;
			Mesh mesh;
			for (auto triIdx : indices)
			{
				vector<Vertex> triangle;

				for (int i = 0; i < 3; i++)
				{
					Vertex v;
					v.position = pos[triIdx[i]];
					//v.texCoord = texcoords[triIdx[i] % 4];
					triangle.push_back(v);
				}

				auto v1 = triangle[1].position - triangle[0].position;
				auto v2 = triangle[2].position - triangle[0].position;
				vec3 norm = normalize(cross(v1, v2));
				for (auto &v : triangle)
				{
					v.normal = norm;
				}

				for (auto &e : triangle)
				{
					vertices.push_back(e);
				}
			}
			mesh.vertices = vertices;
			for (unsigned int i = 0; i < 36; i++)
			{
				mesh.indices.push_back(i);
				mesh.vertices[i].texCoord = texcoords[i % 6];
			}
			return mesh;
		}

	private:
		bool vertDirty = true;
		unsigned int m_vao, m_vbo, m_ebo;
		void clearBuffer()
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ebo);
		}
		void setupMesh()
		{
			vertDirty = false;
			clearBuffer();
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
			glGenBuffers(1, &m_ebo);

			glBindVertexArray(m_vao);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
				GL_STATIC_DRAW);

			if (indices.size() > 0)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					&indices[0], GL_STATIC_DRAW);
			}
			
	
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	
			glEnableVertexAttribArray(1);
			auto offset = offsetof(Vertex, normal);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			glEnableVertexAttribArray(2);
			offset = offsetof(Vertex, texCoord);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

			glBindVertexArray(0);
		}
	};

}