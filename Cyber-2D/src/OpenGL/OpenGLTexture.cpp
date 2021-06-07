#include "pch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>
#include "OpenGL/OpenGLErrorCallback.h"
namespace Cyber {
	Texture::Texture(uint32_t width, uint32_t height, GLenum internalFormat, GLenum dataFormat)
		: m_Width(width), m_Height(height), m_InternalFormat(internalFormat), m_DataFormat(dataFormat)
	{
		GL_CHECK(glGenTextures(1, &m_ID));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_ID));

		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_INT, NULL));

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}

	Texture::Texture(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//CB_CORE_ASSERT(data, "Failed to load image!");
		if (!data) {
			m_Width = 0;
			m_Height = 0;
			return;
		}
		m_Width = width;
		m_Height = height;

		m_InternalFormat = m_DataFormat = 0;
		switch (channels) {
		case 1:
			m_InternalFormat = GL_R8;
			m_DataFormat = GL_RED;
			break;
		case 2:
			m_InternalFormat = GL_RG8;
			m_DataFormat = GL_RG;
			break;
		case 3:
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
			break;
		case 4:
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
			break;
		}

		//CB_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");
		if (!(m_InternalFormat & m_DataFormat)) {
			m_Width = 0;
			m_Height = 0;
			stbi_image_free(data);
			return;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_INT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	uint8_t Texture::GetChannels() const{
		uint8_t bpp = 0;
		switch (m_DataFormat) {
		case GL_RED:
			bpp = 1;
			break;
		case GL_RG:
			bpp = 2;
			break;
		case GL_RGB:
			bpp = 3;
			break;
		case GL_RGBA:
			bpp = 4;
			break;
		}
		return bpp;
	}

	void Texture::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = GetChannels();
		CB_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data));
	}

	void Texture::Bind(uint32_t slot) const
	{
		GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_ID));
	}
}