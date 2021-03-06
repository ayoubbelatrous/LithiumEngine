#include "lipch.h"
#include "LayerStack.h"


namespace Lithium
{
	void LayerStack::PushLayer(Layer* layer)
	{
		_Layers.emplace(_Layers.begin() + _LayerIndex, layer);
		_LayerIndex++;
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		_Layers.emplace_back(layer);
	}

}
