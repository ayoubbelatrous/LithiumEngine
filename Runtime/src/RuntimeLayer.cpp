#include "RuntimeLayer.h"
#include "Input/Input.h"

namespace Lithium {

	void RuntimeLayer::OnCreate()
	{

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

		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		shader = CreateRef<Shader>("assets/shaders/test.shader");
		frameshader = CreateRef<Shader>("assets/shaders/frame.shader");
		

		_vertexarray = CreateRef<VertexArray>();
		_vertexbuffer = CreateRef<VertexBuffer>((void*)&rectangleVertices,sizeof(rectangleVertices));
		Ref<VertexBufferLayout> layout = CreateRef<VertexBufferLayout>();
		//position
		layout->Push<float>(2);
		//texture coordinates
		layout->Push<float>(2);

		_vertexarray->AddBuffer(_vertexbuffer, layout);

		_framebuffer = CreateRef<FrameBuffer>();
		_framebuffer->Bind();

		glm::vec2 size = Application::GetInstance().GetWindow().getSize();
		_framebuffer->resize((int)size.x, (int)size.y);
		proj = glm::perspective(glm::radians(45.0f), (float)size.x / (float)size.y, 0.1f, 100.0f);
		//view = glm::mat4(1.0f);
		pos = glm::vec3(0.0f, 0.0f, -10.0f);
		//view = glm::translate(view, pos);
		//model = glm::translate(glm::mat4(1), pos);


		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		view = glm::mat4(1.0f);
		view = glm::translate(view,pos);
		mesh = Mesh::LoadMesh("assets/model/teapot.obj");
	}

	void RuntimeLayer::OnUpdate()
	{
		rot++;
		float speed = 0.01f;
		if (Input::IsKeyPressed(Key::W))
		{
			view = glm::translate(view, { 0,1 * speed,0 });
		}

		if (Input::IsKeyPressed(Key::S))
		{
			view = glm::translate(view, { 0,-1 * speed,0 });
		}

		if (Input::IsKeyPressed(Key::A))
		{
			view = glm::translate(view, { -1 * speed,0,0 });
		}

		if (Input::IsKeyPressed(Key::D))
		{
			view = glm::translate(view, { 1 * speed,0,0 });
		}
		_framebuffer->Bind();
		RendererCommand::Clear();
		RendererCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f));
		//view = glm::mat4(1.0f);

		//view = glm::translate(view, pos);
		shader->Bind();
		shader->SetUniformMat4f("u_projection", proj * view * model );
		mesh->Render();
		_framebuffer->UnBind();

		frameshader->Bind();
		_framebuffer->BindTexture();
		//frameshader->SetUniform1i("u_tex", _framebuffer->GetColorAttachmentID());

		_vertexarray->Bind();
		_vertexbuffer->Bind();
		RendererCommand::Draw(6);
		//model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-2.0f / 50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	}

	void RuntimeLayer::OnDestroy()
	{

	}

	void RuntimeLayer::onEvent(Event& e)
	{
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& ev = static_cast<WindowResizeEvent&>(e);
			float aspect = ev.GetWidth() / ev.GetHeight();

			proj = glm::perspective(glm::radians(45.0f), (float)ev.GetWidth() / (float)ev.GetHeight(), 0.1f, 100.0f);
			_framebuffer->resize(ev.GetWidth() ,ev.GetHeight());

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