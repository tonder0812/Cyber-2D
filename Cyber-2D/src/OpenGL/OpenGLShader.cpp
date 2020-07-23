#include "pch.h"
#include "OpenGLShader.h"
#include <glad\glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cyber {
	Shader::Shader(const std::string name, const char* vertexSrc, const char* fragmentSrc) :
		m_Name(name), m_File("code")
	{
		std::unordered_map<unsigned int, const char*> srcs;
		srcs[GL_VERTEX_SHADER] = vertexSrc;
		srcs[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(srcs);
	}

	void Shader::Bind() {
		glUseProgram(m_Id);
	}

	void Shader::Unbind() {
		glUseProgram(0);
	}

	std::unordered_map<unsigned int, const char*> Shader::Preprocess(const char* src) {
		std::unordered_map<unsigned int, const char*> tmp;
		return tmp;
	}

	void Shader::Compile(std::unordered_map<unsigned int, const char*> srcs) {
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : srcs)
		{
			GLenum type = kv.first;
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = kv.second;
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				CB_CORE_ERROR("{0}", infoLog.data());
				CB_CORE_ERROR("Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_Id = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			CB_CORE_ERROR("{0}", infoLog.data());
			CB_CORE_ERROR("Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}


	void Shader::UploadUniformInt(const char* name, int value) {
		GLint location = glGetUniformLocation(m_Id, name);
		glUniform1i(location, value);
	}
	void Shader::UploadUniformIntArray(const char *name, int* values, uint32_t count) {
		GLint location = glGetUniformLocation(m_Id, name);
		glUniform1iv(location, count, values);
	}

	void Shader::UploadUniformFloat(const char *name, float value) {
		GLint location = glGetUniformLocation(m_Id, name);
		glUniform1f(location, value);
	}
	void Shader::UploadUniformFloat2(const char* name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const char* name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const char* name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::UploadUniformMat3(const char* name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const char* name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
};
