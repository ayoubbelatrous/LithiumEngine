#pragma once
#include "Lithium.h"


namespace Lithium
{

	class RuntimeLayer : public Lithium::Layer
	{
	public:
		RuntimeLayer()
			:Layer("hi")
		{
			//sz = Serializer(_MainScene);
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& e) override;
		void onKeyEvent(KeyEvent& e);
		void RenderImgui();
		void SceneEvent(Event& e);
	private:
	};
}