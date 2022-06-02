#include "lipch.h"
#include "Mesh/Mesh.h"
#include "Renderer/Renderer.h"

namespace Lithium {
	Mesh::Mesh()
	{
		m_VertexArray = CreateRef<VertexArray>();
		m_VertexBuffer = CreateRef<VertexBuffer>(0);
		m_IndexBuffer = CreateRef<IndexBuffer>(0);
		Ref<VertexBufferLayout> vertexBufferLayout = CreateRef<VertexBufferLayout>();
		vertexBufferLayout->Push<float>(3);
		vertexBufferLayout->Push<float>(3);
		vertexBufferLayout->Push<float>(2);
		vertexBufferLayout->Push<float>(1);
		m_VertexArray->AddBuffer(m_VertexBuffer, vertexBufferLayout);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Render()
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		if (IndexCount == 0)
		{
			RendererCommand::DrawIndexed(0);
		}
	}

	void Mesh::PushVertices(const std::vector<glm::vec3>& Vertices)
	{
		m_VertexBuffer->SetData(&Vertices[0], Vertices.size());
	}

}
