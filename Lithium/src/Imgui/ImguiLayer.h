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
		void Begin();
		void End();
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& event) override;
		void SetBlockEvent(bool block)
		{
			_BlockEvent = block;
		}
	private:
		bool _BlockEvent = false;
	};
}