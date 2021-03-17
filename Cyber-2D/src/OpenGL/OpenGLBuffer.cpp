#include "pch.h"
#include "OpenGLBuffer.h"
#include "OpenGLErrorCallback.h"
#include "glad\glad.h"

namespace Cyber {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(void* data, uint32_t size) {
		GL_CHECK(glGenBuffers(1, &m_Id));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::VertexBuffer(uint32_t size) {
		GL_CHECK(glGenBuffers(1, &m_Id));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::~VertexBuffer() {
		GL_CHECK(glDeleteBuffers(1, &m_Id));
	}

	void VertexBuffer::Bind(bool bindLayout) const {
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
		if (bindLayout)
			m_Layout.Bind();
	}
	void VertexBuffer::Unbind() const {
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::SetData(void* data, uint32_t size)
	{
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
		GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) :
		m_Count(count) {
		GL_CHECK(glGenBuffers(1, &m_Id));
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer() {
		GL_CHECK(glDeleteBuffers(1, &m_Id));
	}

	void IndexBuffer::Bind() const {
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
	}

	void IndexBuffer::Unbind() const {
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	unsigned int ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}

	void BufferLayout::Bind() const {

		GLuint i = 0;
		void *offset = 0;
		for (BufferElement e : m_Elements) {
			GL_CHECK(glEnableVertexAttribArray(i));
			GL_CHECK(glVertexAttribPointer(i, e.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(e.Type), e.Normalized ? GL_TRUE : GL_FALSE, m_Stride, (const void*)e.Offset));
			i++;
		}
	}

}