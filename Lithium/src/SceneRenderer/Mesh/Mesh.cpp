#include "lipch.h"
#include "SceneRenderer/Mesh/Mesh.h"

namespace Lithium::Rendering
{

	Mesh::Mesh()
	{}

	Mesh::~Mesh()
	{}

	void Mesh::SetVertices(const std::vector<glm::vec3>& vertices)
	{
		m_Vertices = vertices;
	}

	void Mesh::SetIndices(const std::vector<uint32_t>& indices)
	{
		m_Indices = indices;
	}

	void Mesh::SetColors(const std::vector<glm::vec4>& colors)
	{
		m_Colors = colors;
	}

	void Mesh::SetTextureCoordinates(const std::vector<glm::vec2>& uvs)
	{
		m_TextureCoordinates = uvs;
	}
}