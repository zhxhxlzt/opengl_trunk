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
	};
	class Mesh : public Object
	{
		TYPE(yk::Mesh, Object)
	public:
		float offset = 0.01f;
		float cor = 0.01f;
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		Mesh() {}
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
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				string number;
				string name = textures[i].type;
				if (name == "texture_diffuse")
					number = to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = to_string(specularNr++);

				
				
				//glCheckError();

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				auto unistr = (name + number);
				shader.set(move(unistr), i);
				glCheckError();
			}
			/*cor += offset;
			if (cor >= 1)
				cor = 0.01f;
			shader.set("cor", cor);
			shader.set("texture_diffuse1", textures[0].id);*/
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
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