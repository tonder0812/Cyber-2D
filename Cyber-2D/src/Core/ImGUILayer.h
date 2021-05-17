#pragma once
#include "Layer.h"
#include "Events/Events.h"

namespace Cyber {
	class ImGUILayer : public Layer
	{
	public:
		ImGUILayer() :Layer("ImGui Layer") {};
		void onAttach() override;
		void onDetach() override;
		bool onEvent(const Event* e) override;
		void Begin();
		void End();
	private:
		std::string m_iniPath;
	};
}

