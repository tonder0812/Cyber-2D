#pragma once
#include "pch.h"
#include "Events\Events.h"

namespace Cyber {
	class Layer;

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		void popOverlay(Layer* layer);
		void onUpdate(float ts);
		void onEvent(const Event *e);
		void onImGUI();
	private:
		std::vector<Layer*> m_Stack;
		std::vector<Layer*> m_Overlays;
	};
	class Layer
	{
	public:
		Layer(std::string nam="Layer"):
		name(nam){
		}
		std::string name;
		virtual void onUpdate(float ts) {};
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