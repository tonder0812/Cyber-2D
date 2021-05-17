#pragma once
#include <string>
#include "entt.hpp"
#include "Renderer/EditorCamera.h"
#include "OpenGL/OpenGLTexture.h"

namespace Cyber {

	class Entity;

	class Scene
	{
	public:
		Scene(bool empty = false);
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(float ts, EditorCamera& editorCamera, std::shared_ptr<Texture> cameraTexture);
		void OnUpdateRuntime(float ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		bool Valid(entt::entity e) { return m_Registry.valid(e); }
		void ResizeCameras();
		void SetPrimaryCamera(Entity e);
	private:
		void Scene::RenderSprites();
		Entity Scene::GetPrimaryCameraEntity();
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight;
		int m_Nentities=0;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
