#include "pch.h"
#include "OpenGLRenderer.h"
#include "OpenGLErrorCallback.h"
#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Cyber {
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		float Channels;
	};
	struct RendererData
	{
		VertexBuffer* vertexBuff;
		IndexBuffer* indexBuff;
		Shader* shader;
		Texture* WhiteTexture;
		const uint32_t MaxQuads = 20000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		const uint32_t MaxTextureSlots = 16;
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;

		Texture* TextureSlots[16];
		uint32_t TextureSlotIndex = 1;
		glm::vec4 QuadVertexPositions[4];
	};
	static RendererData* s_Data;

	//Config
	void Renderer::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		s_Data = new RendererData();
		s_Data->vertexBuff = new VertexBuffer(s_Data->MaxVertices * sizeof(QuadVertex));
		s_Data->vertexBuff->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Float, "a_Channels" }
			});


		uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data->indexBuff = new IndexBuffer(quadIndices, s_Data->MaxIndices);
		s_Data->WhiteTexture = new Texture(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, 3);
		s_Data->WhiteTexture->Bind();

		int32_t* samplers = new int32_t[s_Data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data->shader = new Shader("assets/shaders/Texture.glsl");
		s_Data->shader->Bind();
		s_Data->shader->UploadUniformIntArray("u_Textures", samplers, s_Data->MaxTextureSlots);
		delete[] quadIndices;

		s_Data->TextureSlots[0] = s_Data->WhiteTexture;

		s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}
	void Renderer::Shutdown()
	{
		delete s_Data->indexBuff;
		delete s_Data->vertexBuff;
		delete s_Data->shader;
		delete s_Data->WhiteTexture;
		delete[] s_Data->QuadVertexBufferBase;
		delete s_Data;
	}

	//Batching
	void Renderer::BeginScene(Camera& camera) {
		s_Data->shader->Bind();
		s_Data->shader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}
	void Renderer::EndScene() {
		Flush();
	}

	void Renderer::StartBatch()
	{
		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

	void Renderer::Flush()
	{
		if (s_Data->QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
		s_Data->vertexBuff->SetData(s_Data->QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
			s_Data->TextureSlots[i]->Bind(i);

		Renderer::DrawIndexed(s_Data->vertexBuff, s_Data->indexBuff);
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	/*
	###########
	Primitives
	###########
	*/
	//Filled
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	//Filled rotated
	void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	//Filled Transform
	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = 0;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr->Channels = 4;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
	}


	//Texture
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, Texture* texture, float tilingFactor, const glm::vec4& tintColor)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	//Texture rotated
	void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Texture* texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture* texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	//Texture transform
	void Renderer::DrawQuad(const glm::mat4& transform, Texture* texture, float tilingFactor, const glm::vec4& tintColor)
	{

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= s_Data->MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++)
		{
			if (*(s_Data->TextureSlots[i]) == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data->TextureSlotIndex >= s_Data->MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = tintColor;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr->Channels = texture->GetChannels();
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
	}


	//Commands
	void Renderer::SetClearColor(glm::vec4 color)
	{
		GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
	}
	void Renderer::Clear()
	{
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		GL_CHECK(glViewport(0, 0, width, height));
	}
	void Renderer::DrawIndexed(VertexBuffer* vb, IndexBuffer* ib)
	{
		vb->Bind(true);
		ib->Bind();
		uint32_t count = ib->GetCount();
		GL_CHECK(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}
}