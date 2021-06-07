#pragma once
#include <glm\glm.hpp>
#include "OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Renderer/EditorCamera.h"


namespace Cyber {
	class EditorLayer : public Layer {
	public:
		EditorLayer() :
			Layer("Editor Layer"), m_EditorCamera(EditorCamera(15.0f, 1.778f, -10000.0f, 10000.0f)){
		};
		void onAttach() override;
		void onDetach() override;
		void onImGUI() override;
		void onUpdate(float ts) override;
		bool onEvent(const Event* e) override;
	private:
		void EditorLayer::NewScene();
		void EditorLayer::OpenScene();
		void EditorLayer::SaveSceneAs();
		void EditorLayer::MenuBar();
		void StartDockspace();
	private:
		Scene* m_CurrentScene;
		glm::vec2 m_ViewportSize = { 1,1 };
		glm::vec2 m_ViewportSizeScene;
		EditorCamera m_EditorCamera;
		Framebuffer* m_Framebuffer = nullptr;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		std::shared_ptr<Texture> cameraTexture;
		std::shared_ptr<Texture> PlayButton;
		std::shared_ptr<Texture> PauseButton;
		std::shared_ptr<Texture> StopButton;
		bool m_Runtime = false;
		Entity m_HoveredEntity;
		//Gizmos
		int m_GizmoType = -1;
		//Settings
		int tmpFontSize;
		//Stats
		float framearate = 1;
		int framecount = 0;
	};
}