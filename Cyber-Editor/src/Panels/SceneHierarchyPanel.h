#pragma once

#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Cyber {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Scene* scene);

		void SetContext(Scene* scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Scene* m_Context;
		Entity m_SelectionContext;
	};

}
