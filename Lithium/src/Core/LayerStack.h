#pragma once
#include "Layer.h"
#include <vector>

namespace Lithium
{
	class LayerStack
	{
	private:
		std::vector<Layer*> _Layers;
	public:
		LayerStack() = default;
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		std::vector<Layer*>::iterator begin() { return _Layers.begin(); }
		std::vector<Layer*>::iterator end() { return _Layers.end(); }

		std::vector<Layer*>::reverse_iterator rbegin() { return _Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return _Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return _Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return _Layers.end(); }

		unsigned int _LayerIndex = 0;
	};
}
