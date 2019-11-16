#pragma once
#include "Object.h"
namespace yk
{
	enum WrapMode
	{
		GL_Repeat = GL_REPEAT,
		GL_Mirrored_Repeat = GL_MIRRORED_REPEAT,
		GL_Clamp_To_Edge = GL_CLAMP_TO_EDGE,
		GL_Clamp_To_Border = GL_CLAMP_TO_BORDER
	};

	enum MipmapFilter
	{
		GL_Linear = GL_LINEAR,
		GL_Nearest_Mipmap_Nearest = GL_NEAREST_MIPMAP_NEAREST,
		GL_Linear_Mipmap_Nearest = GL_LINEAR_MIPMAP_NEAREST,
		GL_Nearest_Mipmap_Linear = GL_NEAREST_MIPMAP_LINEAR,
		GL_Linear_Mipmap_Linear = GL_LINEAR_MIPMAP_LINEAR
	};

	class Texture : public Object
	{
		TYPE(yk::Texture, yk::Object);

	public:
		void init(GLenum target = GL_TEXTURE_2D,
			WrapMode wrap_s = GL_Repeat,
			WrapMode wrap_t = GL_Repeat,
			MipmapFilter min_filter = GL_Linear,
			MipmapFilter mag_filter = GL_Linear)
		{
			glGenTextures(1, &m_texture);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter);
		}

		void load(string path, unsigned int rgbMode = GL_RGB, GLenum target = GL_TEXTURE_2D)
		{
			glBindTexture(target, m_texture);
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(target, 0, rgbMode, width, height, 0, rgbMode, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(target);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
		}

		void use(GLuint index, GLenum target = GL_TEXTURE_2D)
		{
			glActiveTexture(index);
			glBindTexture(target, m_texture);
		}

		GLuint getTextureID() { return m_texture; }

	private:
		GLuint m_texture;
	};
}
