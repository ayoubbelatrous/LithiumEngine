#pragma once
#include "imgui.h"
#include "../Core/Layer.h"



namespace Lithium
{
	class GUIlayer : public Layer
	{
	public:
		GUIlayer()
			:Layer("imgui layer")
		{
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:

	};
}