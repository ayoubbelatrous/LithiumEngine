#include "EditorLayer.h"

namespace Lithium
{
	void EditorLayer::OnCreate()
	{

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

		vb = CreateRef<VertexBuffer>(positions, sizeof(positions));
		ibo = CreateRef<IndexBuffer>(sizeof(index), index);
		layout = CreateRef<VertexBufferLayout>();
		layout->Push<float>(2);
		vao = CreateRef<VertexArray>();
		shader = CreateRef<Shader>("assets/shaders/main.shader");

		vao->AddBuffer(vb, layout);

	}

	void EditorLayer::OnUpdate()
	{
		vao->Bind();
		ibo->Bind();

		shader->Bind();
		shader->SetUniform4f("u_color", glm::vec4(0.5, 1.0, 1.0, 1.0));

		RendererCommand::DrawIndexed(6);
	}

	void EditorLayer::OnDestroy()
	{

	}
}
