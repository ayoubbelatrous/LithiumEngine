#include "lipch.h"
#include "SceneRenderer/Mesh/Mesh.h"

namespace Lithium::Rendering
{

	MeshResource::MeshResource()
	{}

	MeshResource::~MeshResource()
	{}

	void MeshResource::SetVertices(const std::vector<glm::vec3>& vertices)
	{
		m_Vertices = vertices;
	}

	void MeshResource::SetIndices(const std::vector<uint32_t>& indices)
	{
		m_Indices = indices;
	}

	void MeshResource::SetColors(const std::vector<glm::vec4>& colors)
	{
		m_Colors = colors;
	}

	void MeshResource::SetTextureCoordinates(const std::vector<glm::vec2>& uvs)
	{
		m_TextureCoordinates = uvs;
	}

	std::vector<Lithium::Rendering::MeshRecourceVertex> MeshResource::GetVertexData() const
	{
		return m_MeshRecourceVertexData;
	}

	void MeshResource::CookVertexData()
	{
		//clear cooked resource vertex data from previous cooks

		m_MeshRecourceVertexData.clear();

		for (size_t i = 0; i < m_Vertices.size(); i++)
		{
			glm::vec3 pos = m_Vertices[i];
			glm::vec4 color = m_Colors[i];
			glm::vec2 uv = m_TextureCoordinates[i];

			m_MeshRecourceVertexData.push_back({ pos,color,uv });
		}

		//clear temporary mesh data as they are no longer needed

		m_Vertices.clear();
		m_Colors.clear();
		m_TextureCoordinates.clear();
	}

	std::vector<uint32_t> MeshResource::GetIndexData() const
	{
		return m_Indices;
	}

}