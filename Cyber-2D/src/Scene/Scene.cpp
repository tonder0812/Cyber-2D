#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Utils/Utils.h"
#include "OpenGL/OpenGLRenderer.h"
#include <Imgui.h>
#include <imgui_internal.h>
#include <glm/glm.hpp>

namespace Cyber {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}


	void Scene::OnUpdateEditor(float ts)
	{
		/*auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			if (sprite.UseTexture) {
				Renderer::DrawQuad(transform.GetTransform(), sprite.texture.get(), 0, sprite.Color);
			}
			else {
				Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
			}
		}*/
		m_Registry.each([&](auto entityID)
			{
				Entity entity = Entity(entityID, this);
				if (entity.HasComponent<SpriteRendererComponent>()) {
					TransformComponent& transform = entity.GetComponent<TransformComponent>();
					SpriteRendererComponent& sprite = entity.GetComponent<SpriteRendererComponent>();
					if (sprite.UseTexture) {
						Renderer::DrawQuad(transform.GetTransform(), sprite.texture.get(), 1, sprite.Color);
					}
					else {
						Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
					}
				}
			});
	}

	void Scene::OnImGui() {
		//Scene hierarchy
		ImGui::Begin("Scene hierarchy");
		m_Registry.each([&](auto entityID)
			{
				Entity* entity = new Entity(entityID, this);
				std::string tag = entity->GetComponent<TagComponent>().Tag;
				if (ImGui::Button(tag.c_str())) {
					if (m_SelectedEntity) {
						delete m_SelectedEntity;
					}
					m_SelectedEntity = entity;
				}
				else {
					delete entity;
				}
			});
		ImGui::End();
		//Properties
		ImGui::Begin("Properties");
		if (m_SelectedEntity == nullptr) {
			ImGui::End();
			return;
		}
		std::string& tag = m_SelectedEntity->GetComponent<TagComponent>().Tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
		ImGui::Text("Transform");
		TransformComponent& transform = m_SelectedEntity->GetComponent<TransformComponent>();
		ImGui::DragFloat3("Translation", &transform.Translation.x);
		float rotation = glm::degrees(transform.Rotation);
		if (ImGui::DragFloat("Rotation", &rotation)) {
			transform.Rotation = glm::radians(rotation);
		}
		ImGui::DragFloat3("Scale", &transform.Scale.x);
		if (m_SelectedEntity->HasComponent<SpriteRendererComponent>())
		{
			ImGui::Text("Sprite");
			SpriteRendererComponent& sprite = m_SelectedEntity->GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", &sprite.Color.r);
			bool disabled = sprite.texture.get() == nullptr;
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (disabled ? 0.5f : 1.0f));
			ImGui::Checkbox("Use texture", &sprite.UseTexture);
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
			if (ImGui::Button("Load Image"))
			{
				std::string path = FileDialogs::OpenFile("Image Files(*.BMP;*.JPG;*.PNG)\0*.BMP;*.JPG;*.PNG\0All files (*.*)|*.*\0");
				if (!path.empty()) {
					auto texture = std::make_shared<Texture>(path);
					sprite.texture = texture;
				}
			}
		}
		ImGui::End();
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

}
