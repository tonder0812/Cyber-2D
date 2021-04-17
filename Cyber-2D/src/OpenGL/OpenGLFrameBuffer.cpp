#include "pch.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLErrorCallback.h"
#include "glad/glad.h"

namespace Cyber {
	static const uint32_t s_MaxFramebufferSize = 8192;

	static void AttachColorTexture(uint32_t id, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0));
	}

	static void AttachDepthTexture(uint32_t id, GLenum internalformat, GLenum attachmentType, GLenum format, GLenum type, uint32_t width, uint32_t height)
	{
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, NULL));

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0));
	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
		}

		return false;
	}

	static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}

		CB_CORE_ASSERT(false, "Invalid FramebufferTextureFormat");
		return 0;
	}

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		GL_CHECK(glDeleteFramebuffers(1, &m_RendererID));
		GL_CHECK(glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));
		GL_CHECK(glDeleteTextures(1, &m_DepthAttachment));
	}

	void Framebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			GL_CHECK(glDeleteFramebuffers(1, &m_RendererID));
			GL_CHECK(glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));
			GL_CHECK(glDeleteTextures(1, &m_DepthAttachment));

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		GL_CHECK(glGenFramebuffers(1, &m_RendererID));
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));


		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			GL_CHECK(glGenTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]));
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(m_ColorAttachments[i], GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_ColorAttachments[i], GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			GL_CHECK(glGenTextures(1, &m_DepthAttachment));

			GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_DepthAttachment, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			CB_CORE_ASSERT(m_ColorAttachments.size() <= 4, "To many color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			GL_CHECK(glDrawBuffers(m_ColorAttachments.size(), buffers));
		}
		else if (m_ColorAttachments.empty())
		{
			GL_CHECK(glDrawBuffer(GL_NONE));
		}

		CB_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void Framebuffer::Bind()
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		GL_CHECK(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
	}

	void Framebuffer::Unbind()
	{
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			CB_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		CB_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "attachmentIndex out of range");

		GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
		int pixelData;
		GL_CHECK(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
		return pixelData;

	}

	void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		CB_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "attachmentIndex out of range");

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		GL_CHECK(glClearTexImage(m_ColorAttachments[attachmentIndex], 0, FBTextureFormatToGL(spec.TextureFormat), GL_INT, &value));
	}

}