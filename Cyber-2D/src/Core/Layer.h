#pragma once
#include "pch.h"
#include "Events\Events.h"

namespace Cyber {
	class Layer;
	class Application;

	class LayerStack
	{
	public:
		LayerStack(Application* app=nullptr);
		~LayerStack();
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void onUpdate();
		void onEvent(const Event *e);
		void onImGUI();
		bool Close();
	private:
		Application* m_Application;
		std::vector<Layer*> m_Stack;
	};
	class Layer
	{
	public:
		Layer(std::string nam="Layer"):
		name(nam){
		}
		std::string name;
		virtual void onUpdate() {};
		virtual void onImGUI() {};
		virtual void onAttach() {};
		virtual void onDetach() {};
		virtual bool onEvent(const Event* e) { return false; };
	private:
		LayerStack* m_Stack;
		bool Close() {
			return m_Stack->Close();
		};

	};
}
