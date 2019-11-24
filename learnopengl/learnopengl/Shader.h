#pragma once

#include "Object.h"


namespace yk
{
	class Shader : public Object
	{
		TYPE(yk::Shader, Object);

	public:
		Shader() = default;
		Shader(string vShaderPath, string fShaderPath)
		{
			auto vCode = loadShaderScript(vShaderPath);
			auto fCode = loadShaderScript(fShaderPath);
			init(vCode.c_str(), fCode.c_str());
		}

		void init(const char* vCode, const char* fCode)
		{
			glDeleteProgram(m_shaderProgram);
			auto vertex = glCreateShader(GL_VERTEX_SHADER);
			auto frag = glCreateShader(GL_FRAGMENT_SHADER);
			
			glShaderSource(vertex, 1, &vCode, NULL);
			glShaderSource(frag, 1, &fCode, NULL);

			glCompileShader(vertex);
			glCompileShader(frag);

			checkCompileErrors(vertex, "VERTEX");
			checkCompileErrors(frag, "FRAGMENT");

			auto ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, frag);
			glLinkProgram(ID);
			checkCompileErrors(ID, "PROGRAM");
			m_shaderProgram = ID;

			glDeleteShader(vertex);
			glDeleteShader(frag);
		}

		void initByPath(string vShaderPath, string fShaderPath)
		{
			auto vCode = loadShaderScript(vShaderPath);
			auto fCode = loadShaderScript(fShaderPath);
			init(vCode.c_str(), fCode.c_str());
		}
		
		void use()
		{
			glUseProgram(m_shaderProgram);
		}

		template<typename T>
		void set(const string &&name, T value) const
		{
			auto loc = glGetUniformLocation(m_shaderProgram, name.c_str());
			glUniform1i(loc, (GLint)value);
		}

		template<>
		void set<float>(const string &&name, float value) const
		{
			auto loc = glGetUniformLocation(m_shaderProgram, name.c_str());
			glUniform1f(loc, value);
		}
		
		template<>
		void set(const string &&name, glm::mat4 mtx) const
		{
			auto loc = glGetUniformLocation(m_shaderProgram, name.c_str());
			glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(mtx));
		}
		
	private:
		GLint m_shaderProgram = -1;

	private:
		string loadShaderScript(string path)
		{
			ifstream f(path);
			auto info = (stringstream() << f.rdbuf()).str();
			return move(info);
		}

		void checkCompileErrors(unsigned int shader, std::string type)
		{
			int success;
			char infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}
	};

}