#pragma once
#include "Lithium.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Renderer/FrameBuffer.h"
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
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 pos;
		Ref<FrameBuffer> _framebuffer;
	};
}