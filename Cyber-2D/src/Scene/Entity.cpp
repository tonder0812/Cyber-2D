#include "pch.h"
#include "Entity.h"
#include "Components.h"

namespace Cyber {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
	template<typename T>
	void Entity::RemoveComponent()
	{
		CB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	template<>
	void Entity::RemoveComponent<TransformComponent>()
	{
		CB_CORE_ASSERT(HasComponent<TransformComponent>(), "Entity does not have a Transform component!");
		GetComponent<TransformComponent>().Destroy();
		m_Scene->m_Registry.remove<TransformComponent>(m_EntityHandle);
	}

	template<>
	void Entity::RemoveComponent<CameraComponent>()
	{
		CB_CORE_ASSERT(HasComponent<CameraComponent>(), "Entity does not have a Camera component!");
		m_Scene->m_Registry.remove<CameraComponent>(m_EntityHandle);
	}

	template<>
	void Entity::RemoveComponent<ScriptComponent>()
	{
		CB_CORE_ASSERT(HasComponent<ScriptComponent>(), "Entity does not have a Script component!");
		GetComponent<ScriptComponent>().Destroy();
		m_Scene->m_Registry.remove<ScriptComponent>(m_EntityHandle);
	}

	template<>
	void Entity::RemoveComponent<SpriteRendererComponent>()
	{
		CB_CORE_ASSERT(HasComponent<SpriteRendererComponent>(), "Entity does have a not Sprite Renderer component!");
		GetComponent<SpriteRendererComponent>().Destroy();
		m_Scene->m_Registry.remove<SpriteRendererComponent>(m_EntityHandle);
	}
}