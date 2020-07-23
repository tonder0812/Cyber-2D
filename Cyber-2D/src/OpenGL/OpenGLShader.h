#pragma once
#include "pch.h"
#include "glm\glm.hpp"

namespace Cyber {
	class Shader
	{
	public:
		Shader(const std::string name,const char* vertexSrc,const char* fragmentSrc);
		void Bind();
		void Unbind();



		const std::string& GetName() const { return m_Name; }
		const std::string& GetFile() const { return m_File; }
		void UploadUniformInt(const char* name, int value);
		void UploadUniformIntArray(const char* name, int* values, uint32_t count);

		void UploadUniformFloat(const char* name, float value);
		void UploadUniformFloat2(const char* name, const glm::vec2& value);
		void UploadUniformFloat3(const char* name, const glm::vec3& value);
		void UploadUniformFloat4(const char* name, const glm::vec4& value);

		void UploadUniformMat3(const char* name, const glm::mat3& matrix);
		void UploadUniformMat4(const char* name, const glm::mat4& matrix);
	private:
		std::unordered_map<unsigned int, const char*> Preprocess(const char* src);
		void Compile(std::unordered_map<unsigned int, const char*> srcs);
		std::string m_Name,m_File;
		uint32_t m_Id;
	};
}


inline std::ostream& operator<<(std::ostream& os, const Cyber::Shader& shader)
{
	return os << shader.GetName() << " From: " << shader.GetFile();
}
