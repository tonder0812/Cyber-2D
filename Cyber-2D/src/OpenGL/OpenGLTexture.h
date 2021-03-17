#pragma once
typedef unsigned int GLenum;
namespace Cyber {
	class Texture
	{
	public:
		Texture(uint32_t width, uint32_t height, GLenum internalFormat = 0x8058, GLenum dataFormat = 0x1907);
		Texture(const std::string& path);
		~Texture();
		uint32_t GetWidth() const { return m_Width; };
		uint32_t GetHeight() const { return m_Height; };
		uint32_t GetID() const { return m_ID; };
		uint8_t GetChannels() const;
		void SetData(void* data, uint32_t size);
		void Bind(uint32_t slot = 0) const;
		bool operator==(const Texture& other) const { return m_ID == other.m_ID; };
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_ID = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};
}