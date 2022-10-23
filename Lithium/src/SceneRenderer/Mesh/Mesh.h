#pragma once


namespace Lithium::Rendering
{
	enum class MeshType
	{
		Static,Dynamic
	};

	class Mesh
	{
	public:

		Mesh();
		~Mesh();
		void SetVertices(const std::vector<glm::vec3>& vertices);
		void SetIndices(const std::vector<uint32_t>& indices);
		void SetColors(const std::vector<glm::vec4>& colors);
		void SetTextureCoordinates(const std::vector<glm::vec2>& uvs);
	private:
		std::vector<glm::vec3> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<glm::vec4> m_Colors;
		std::vector<glm::vec2> m_TextureCoordinates;
	};
}