#pragma once
#include "pch.h"
#include "Events\Events.h"

namespace Cyber {
	class Layer;
	class Application;

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void onUpdate();
		void onEvent(const Event *e);
		void onImGUI();
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
	protected:
		bool Close();
	};
}

inline std::ostream& operator<< (std::ostream& os, const Cyber::Layer& l) {
	os << l.name;
	return os;
}