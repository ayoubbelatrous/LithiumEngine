#include "RuntimeLayer.h"
#include "Renderer/Renderer2D.h"

namespace Lithium {

	void RuntimeLayer::OnCreate()
	{
		//_framebuffer = CreateRef<FrameBuffer>();
		//_framebuffer->Bind();
		//_framebuffer->resize(1000, 1000);
		pos = glm::vec3(0);
		view = glm::mat4(0);
		view = model = glm::translate(glm::mat4(1), glm::vec3(1));

		proj = glm::ortho(-2.0, 2.0, -2.0, 2.0);
		model = glm::translate(glm::mat4(1), pos);

		BatchRenderer::Init();
	}

	void RuntimeLayer::OnUpdate()
	{
		//RendererCommand::ClearColor({ 0.25,0.25,0.24,0.25 });
	
		//_framebuffer->Bind();
		BatchRenderer::Begin(view, proj);
	

		BatchRenderer::End(); 
		//RendererCommand::Clear();
		//_framebuffer->UnBind();
	}

	void RuntimeLayer::OnDestroy()
	{

	}

	void RuntimeLayer::onEvent(Event& e)
	{

	}

	void RuntimeLayer::onKeyEvent(KeyEvent& e)
	{

	}

	void RuntimeLayer::RenderImgui()
	{

	}

	void RuntimeLayer::SceneEvent(Event& e)
	{

	}

}