#include "EditorLayer.h"


namespace Lithium
{
	void EditorLayer::OnCreate()
	{
		LT_PROFILE_FUNCTION("init");
		float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
		};
		unsigned int index[] = {
			0, 1, 2,
			2, 3, 0
		};

		pos = glm::vec3(0);
		view = glm::mat4(0);
		view = model = glm::translate(glm::mat4(1), glm::vec3(1));

		proj = glm::ortho(-2.0,2.0,-2.0,2.0);
		model = glm::translate(glm::mat4(1), pos);
		tex = CreateRef<Texture>("assets/images/check.png");
		tex2 = CreateRef<Texture>("assets/images/eda.png");
		Renderer2D::Init();
	}

	void EditorLayer::OnUpdate()
	{
		LT_PROFILE_FUNCTION("update");
		RendererCommand::ClearColor(glm::vec4(0.25));
		RendererCommand::Clear();
		

		Renderer2D::BeginScene(proj, view);

		
		//Renderer2D::DrawQuad(model, glm::vec4(1));
		model = glm::translate(glm::mat4(1), pos);
		
		{
			LT_PROFILE_SCOPE("render 300 quads");
			model = glm::translate(glm::mat4(1), { 0.0,0.0,0.0 });
			Renderer2D::DrawQuad(model, glm::vec4(1.0, 0.0, 0.0, 1.0));
		}
	
		//Renderer2D::DrawQuad(model, glm::vec4(1.0, 1.0, 1.0, 1.0), tex);
		Renderer2D::EndScene();
	}

	void EditorLayer::OnDestroy()
	{

	}

}
