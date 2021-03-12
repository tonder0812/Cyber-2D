#include "pch.h"
#include "OpenGLShader.h"
#include "OpenGLErrorCallback.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace Cyber {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		CB_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	Shader::Shader(const std::string& filepath):
		m_File(filepath)
	{

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	Shader::Shader(const std::string& name, const std::string vertexSrc, const std::string fragmentSrc) :
		m_Name(name), m_File("code")
	{
		std::unordered_map<unsigned int, std::string> srcs;
		srcs[GL_VERTEX_SHADER] = vertexSrc;
		srcs[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(srcs);
	}

	void Shader::Bind() {
		GL_CHECK(glUseProgram(m_Id));
	}

	void Shader::Unbind() {
		GL_CHECK(glUseProgram(0));
	}

	std::unordered_map<unsigned int, std::string> Shader::PreProcess(std::string source) {
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			CB_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			CB_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			CB_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = ((pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos));
			//CB_CORE_WARN("{0} {1}",type,shaderSources[ShaderTypeFromString(type)]);
		}

		return shaderSources;
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				CB_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			CB_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	void Shader::Compile(std::unordered_map<unsigned int, std::string> srcs) {
		GLuint program = glCreateProgram();
		CB_CORE_ASSERT(srcs.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : srcs)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			GL_CHECK(glShaderSource(shader, 1, &sourceCStr, 0));

			GL_CHECK(glCompileShader(shader));

			GLint isCompiled = 0;
			GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

				std::vector<GLchar> infoLog(maxLength);
				GL_CHECK(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

				GL_CHECK(glDeleteShader(shader));

				CB_CORE_ERROR("{0}", infoLog.data());
				CB_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			GL_CHECK(glAttachShader(program, shader));
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_Id = program;

		// Link our program
		GL_CHECK(glLinkProgram(program));

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GL_CHECK(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

			// We don't need the program anymore.
			GL_CHECK(glDeleteProgram(program));

			for (auto id : glShaderIDs)
				GL_CHECK(glDeleteShader(id));

			CB_CORE_ERROR("{0}", infoLog.data());
			CB_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			GL_CHECK(glDetachShader(program, id));
			GL_CHECK(glDeleteShader(id));
		}
	}


	void Shader::UploadUniformInt(const char* name, int value) {
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniform1i(location, value));
	}
	void Shader::UploadUniformIntArray(const char* name, int* values, uint32_t count) {
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniform1iv(location, count, values));
	}

	void Shader::UploadUniformFloat(const char* name, float value) {
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniform1f(location, value));
	}
	void Shader::UploadUniformFloat2(const char* name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniform2f(location, value.x, value.y));
	}

	void Shader::UploadUniformFloat3(const char* name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniform3f(location, value.x, value.y, value.z));
	}

	void Shader::UploadUniformFloat4(const char* name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	void Shader::UploadUniformMat3(const char* name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void Shader::UploadUniformMat4(const char* name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_Id, name);
		GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}
};
