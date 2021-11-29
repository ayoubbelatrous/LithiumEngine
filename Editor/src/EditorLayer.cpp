#include "EditorLayer.h"
namespace Lithium
{
	void EditorLayer::OnCreate()
	{
		tex = CreateRef<Texture>(1, 1);
		shader = Shader::Load("assets/shaders/main.shader");
		pos = glm::vec3(0.0);
		quad = glm::translate(glm::mat4(1), pos);
		Renderer2D::Init();
		view = glm::translate(glm::mat4(1),glm::vec3(0));
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f);
		color = glm::vec4(1.0);
	}

	void EditorLayer::OnUpdate()
	{
		Renderer2D::BeginScene(projection, view);
	

		
		Renderer2D::DrawQuad(quad, color);


		Renderer2D::EndScene();
	}

	void EditorLayer::OnDestroy()
	{

	}
}
