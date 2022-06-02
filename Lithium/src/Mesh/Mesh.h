#pragma once
#include "Core/Base.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

namespace Lithium
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		void Render();
		void PushVertices(const std::vector<glm::vec3>& Vertices);
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		int IndexCount = 0;
	};
}
