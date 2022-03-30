#include "RuntimeLayer.h"
#include "Input/Input.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Serializer.h"
#include "Audio/Audio.h"
#include "Scene/Entity.h"


namespace Lithium {

	
	void RuntimeLayer::OnCreate()
	{
		m_ActiveScene = CreateRef<Scene>();
		Serializer ser(m_ActiveScene);
		ser.DeserializeScene("assets/scenes/platformer.scene");
		Application::Get().sceneManager->SetActiveScene(m_ActiveScene);
		Application::Get().SetVsync(true);
		Audio::Init();
		float rectangleVertices[] =
		{
			// Coords    // texCoordsmesh
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f, 
			-1.0f,  1.0f,  0.0f, 1.0f,

			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};
		
		FrameBufferAttachmentDescriptor framebufferspec(
			{
				FramebufferTextureFormat::RGBA8,
				FramebufferTextureFormat::RED_INTEGER,
			}
		);

		frameshader = CreateRef<Shader>("assets/shaders/frame.shader");

		m_Varray = CreateRef<VertexArray>();
		m_VBuffer = CreateRef<VertexBuffer>((void*)&rectangleVertices, sizeof(rectangleVertices));
		Ref<VertexBufferLayout> layout = CreateRef<VertexBufferLayout>();
		layout->Push<float>(2);
		layout->Push<float>(2);
		m_Varray->AddBuffer(m_VBuffer, layout);


		BatchRenderer::Init();
		m_MainFrameBuffer = CreateRef<FrameBuffer>(framebufferspec);
		glm::vec2 size = Application::Get().GetWindow().getSize();
		m_ActiveScene->OnViewportResize((int)size.x, (int)size.y);
		m_MainFrameBuffer->resize((int)size.x, (int)size.y);
		m_ActiveScene->OnStart();
		m_ActiveScene->SortScene();
		Application::Get().SetVsync(true);

	/*	Entity camera = m_ActiveScene->CreateEntity("Camera");
		camera.AddComponent<CameraComponent>();
		camera.GetComponent<CameraComponent>().ClearColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		Entity sprite = m_ActiveScene->CreateEntity("sprite");
		sprite.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));*/
	}

	void RuntimeLayer::OnUpdate()
	{
		m_MainFrameBuffer->Bind();
		m_ActiveScene->onUpdate();
		m_MainFrameBuffer->UnBind();
		RendererCommand::ClearColor(glm::vec4(0.00, 0.00, 0.00, 0));
		RendererCommand::Clear();
		m_Varray->Bind();
		frameshader->Bind();
		m_MainFrameBuffer->BindTexture(0, 0);
		frameshader->SetUniform1i("u_tex", m_MainFrameBuffer->GetColorAttachmentID());
		RendererCommand::Draw(6);
	}

	void RuntimeLayer::OnDestroy()
	{
		m_ActiveScene->OnStop();

	}

	void RuntimeLayer::onEvent(Event& e)
	{
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& ev = static_cast<WindowResizeEvent&>(e);
			m_MainFrameBuffer->resize(ev.GetWidth() ,ev.GetHeight());
			m_ActiveScene->OnViewportResize(ev.GetWidth(), ev.GetHeight());
		}

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