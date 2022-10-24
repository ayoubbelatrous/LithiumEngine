#pragma once


namespace Lithium::Rendering
{
	struct MeshRecourceVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoordinate;
	};
	class MeshResource
	{
	public:
		MeshResource();
		~MeshResource();
		void SetVertices(const std::vector<glm::vec3>& vertices);
		void SetIndices(const std::vector<uint32_t>& indices);
		void SetColors(const std::vector<glm::vec4>& colors);
		void SetTextureCoordinates(const std::vector<glm::vec2>& uvs);
		void CookVertexData();

		std::vector<MeshRecourceVertex> GetVertexData() const;
		std::vector<uint32_t> GetIndexData() const;

	private:
		std::vector<glm::vec3> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<glm::vec4> m_Colors;
		std::vector<glm::vec2> m_TextureCoordinates;

		std::vector<MeshRecourceVertex> m_MeshRecourceVertexData;
	};
}