#pragma once

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"

#include "Core/UUID.h"

namespace Lithium::Rendering::Data
{
	struct SceneRendererSpecification
	{
		VertexBufferLayout vertexBufferLayout;
	};
	struct SceneRendererData
	{
		std::vector<UUID> MeshIDs;
		std::unordered_map<UUID,VertexBuffer> VertexBuffers;
		std::unordered_map<UUID, IndexBuffer> IndexBuffers;
		std::unordered_map<UUID, VertexArray> VertexArrays;
		VertexBufferLayout vertexBufferLayout;
	};
}