#pragma once
#include "glm/glm.hpp"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "Renderer/Camera.h"
namespace Cyber {

	class Renderer
	{
	public:
		//Config
		static void Init();
		static void Shutdown();

		//Batching 
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Flush();

		/*
		###########
		Primitives
		###########
		*/
		//Filled
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		//Filled rotated
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		//Filled transform
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);


		//Texture
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Texture* texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		//Texture rotated
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Texture* texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture* texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		//Texture transform 
		static void DrawQuad(const glm::mat4& transform, Texture* texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


		//Commands 
		static void SetClearColor(glm::vec4 color);
		static void Clear();
		static void SetViewport(uint32_t width, uint32_t height);
		static void DrawIndexed(VertexBuffer* vb, IndexBuffer* ib);

	private:
		//Batching
		static void StartBatch();
		static void NextBatch();
	};
}

