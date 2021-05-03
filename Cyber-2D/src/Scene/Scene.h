#pragma once
#include <string>
#include "entt.hpp"

namespace Cyber {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(float ts);

		void OnImGui();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		Entity* m_SelectedEntity = nullptr;
		friend class Entity;
	};

}
