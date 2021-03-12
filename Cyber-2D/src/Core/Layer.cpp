#include "pch.h"
#include "Layer.h"
#include "Application.h"

namespace Cyber {
	LayerStack::LayerStack() {};
	LayerStack::~LayerStack() {
		for (Layer* layer : m_Stack) {
			CB_CORE_WARN("LAYER {0} POPPED", *layer);
			layer->onDetach();
			CB_CORE_WARN("LAYER {0} DELETED",*layer);
			delete layer;
		}
		for (Layer* layer : m_Overlays) {
			CB_CORE_WARN("OVERLAY {0} POPPED", *layer);
			layer->onDetach();
			CB_CORE_WARN("OVERLAY {0} DELETED", *layer);
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		m_Stack.emplace_back(layer);
		CB_CORE_WARN("LAYER {0} PUSHED",*layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(Layer* layer) {
		auto it = std::find(m_Stack.begin() , m_Stack.end(), layer);
		if (it!=m_Stack.end())
		{
			layer->onDetach();
			CB_CORE_WARN("LAYER {0} POPPED",*layer);
			m_Stack.erase(it);
			CB_CORE_WARN("LAYER {0} DELETED", *layer);
			delete layer;
		}
	}

	void LayerStack::pushOverlay(Layer* layer)
	{
		m_Overlays.emplace_back(layer);
		CB_CORE_WARN("OVERLAY {0} PUSHED", *layer);
		layer->onAttach();
	}

	void LayerStack::popOverlay(Layer* layer) {
		auto it = std::find(m_Overlays.begin(), m_Overlays.end(), layer);
		if (it != m_Overlays.end())
		{
			CB_CORE_WARN("OVERLAY {0} POPPED", *layer);
			layer->onDetach();
			m_Overlays.erase(it);
			CB_CORE_WARN("OVERLAY {0} DELETED", *layer);
			delete layer;
		}
	}

	void LayerStack::onUpdate(float ts) {
		int i = 0;
		for (auto it = m_Stack.rbegin(); it != m_Stack.rend(); ++it)
		{
			#ifdef CB_DEBUG
			CB_CORE_INFO("Update {0}:{1}", *(*it), i);
			i++;
			#endif
			(*it)->onUpdate(ts);
		}
		for (auto it = m_Overlays.rbegin(); it != m_Overlays.rend(); ++it)
		{
			#ifdef CB_DEBUG
			CB_CORE_INFO("Update {0}:{1}", *(*it), i);
			i++;
			#endif
			(*it)->onUpdate(ts);
		}
	}

	void LayerStack::onImGUI() {
		for (Layer* layer : m_Stack)
			layer->onImGUI();
		for (Layer* layer : m_Overlays)
			layer->onImGUI();
	}

	void LayerStack::onEvent(const Event *e) {
		#ifdef CB_DEBUG
			int i = 0;
		#endif
		for (Layer* layer : m_Overlays) {
			#ifdef CB_DEBUG
			CB_CORE_INFO("Event {0}:{1}", *layer, i);
			i++;
			#endif
			if (layer->onEvent(e))
				break;
		}
		for (Layer* layer : m_Stack) {
			#ifdef CB_DEBUG
			CB_CORE_INFO("Event {0}:{1}", *layer, i);
			i++;
			#endif
			if (layer->onEvent(e))
				break;
		}
	}

	bool Layer::Close() {
		return Application::Get().Close();
	};
}