#include "pch.h"
#include "Layer.h"
#include "Application.h"

namespace Cyber {
	LayerStack::LayerStack(Application* app) :
		m_Application(app) {
	}
	LayerStack::~LayerStack() {
		for (Layer* layer : m_Stack) {
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		m_Stack.emplace_back(layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(Layer* layer) {
		auto it = std::find(m_Stack.begin() , m_Stack.end(), layer);
		if (it!=m_Stack.end())
		{
			layer->onDetach();
			m_Stack.erase(it);
		}
	}

	void LayerStack::onUpdate() {
		for (Layer* layer : m_Stack) {
			//CB_CORE_INFO("LAYER {0} UPDATED", layer->name);
			layer->onUpdate();
		}
	}

	void LayerStack::onImGUI() {
		for (Layer* layer : m_Stack)
			layer->onImGUI();
	}

	void LayerStack::onEvent(const Event *e) {
		for (auto it = m_Stack.rbegin(); it != m_Stack.rend(); ++it)
		{
			if ((*it)->onEvent(e))
				break;
		}
	}

	bool LayerStack::Close() {
		return m_Application->Close();
	}


}