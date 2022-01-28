#pragma once
#include "glm.hpp"
#include "Core/Base.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBufferLayout.h"
namespace Lithium
{
	struct MeshVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};
	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		static Ref<Mesh> LoadMesh(const std::string& path);
		void SetVertices(const std::vector<MeshVertex>& vertices);
		void SetIndices(const std::vector<uint32_t>& indices);
		void SetupMesh();
		void Render();
	private:
		std::vector<MeshVertex> _Vertices;
		std::vector<uint32_t> _Indices;

		Ref<VertexArray> _VertexArray;
		Ref<IndexBuffer> _IndexBuffer;
		Ref<VertexBuffer> _VertexBuffer;

	};
}

