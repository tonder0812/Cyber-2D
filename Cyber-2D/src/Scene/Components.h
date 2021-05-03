#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OpenGL/OpenGLTexture.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Cyber {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{

			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::rotate(glm::mat4(1.0f), Rotation, { 0,0,1 })
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<Texture> texture;
		bool UseTexture = false;
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color), texture(nullptr), UseTexture(false) {}
		SpriteRendererComponent(std::shared_ptr<Texture> tex)
			: Color({ 1,1,1,1 }), texture(tex), UseTexture(true) {}
		SpriteRendererComponent(const glm::vec4& color, std::shared_ptr<Texture> tex)
			: Color(color), texture(tex), UseTexture(true) {}
	};
}
