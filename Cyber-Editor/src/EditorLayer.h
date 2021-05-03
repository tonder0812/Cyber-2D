#pragma once
#include <glm\glm.hpp>
#include "OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"


namespace Cyber {
	class EditorLayer : public Layer {
	public:
		EditorLayer() :
			Layer("Editor Layer"), m_Camera(1, -1, -1, 1, -100, 100) {
		};
		void onAttach() override;
		void onDetach() override;
		void onImGUI() override;
		void onUpdate(float ts) override;
		bool onEvent(const Event* e) override;
	private:
		void StartDockspace();
	private:
		Scene m_CurrentScene;
		Entity m_Square;
		Entity m_Square2;
		glm::vec2 m_ViewportSize = { 1,1 };
		OrthographicCamera m_Camera;
		Framebuffer* m_Framebuffer = nullptr;
	};
}