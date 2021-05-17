#pragma once
#include "Cyber.h"
#include <glm\glm.hpp>
#include "Scene/Scene.h"


namespace Cyber {
	class StandAloneLayer : public Layer {
	public:
		StandAloneLayer() :
			Layer("StandAlone Layer") {
		};
		void onAttach() override;
		void onDetach() override;
		void onImGUI() override;
		void onUpdate(float ts) override;
		bool onEvent(const Event* e) override;
	private:
		glm::vec2 m_ViewportSize = { 0,0 };
		bool m_ViewportSizeChanged = true;
		Scene* m_CurrentScene = nullptr;
	};
}