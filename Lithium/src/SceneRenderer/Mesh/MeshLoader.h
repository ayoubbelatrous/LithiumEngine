#pragma once

namespace Assimp
{
	class Importer;
}

namespace Lithium::Rendering::Loaders
{
	struct MeshLoaderSceneInfo
	{
		uint32_t m_MeshCount;
	};
	class MeshLoader
	{
	public:
		MeshLoader(const std::string& path);
		~MeshLoader();	

		std::vector<glm::vec3> GetVertices() const;
		std::vector<uint32_t> GetIndices() const;
		std::vector<glm::vec4> GetColors() const;
		std::vector<glm::vec2> GetTextureCoordinates() const;
	private:

		void Load();
		void LoadVertices();
		void LoadIndices();
		void LoadColors();
		void LoadTextureCoordinates();
		bool LoadFinished();

		std::atomic_bool m_FinishedLoading = false;
		std::string m_Path;
		std::thread m_LoadThread;

		std::vector<glm::vec3> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<glm::vec4> m_Colors;
		std::vector<glm::vec2> m_TextureCoordinates;

		MeshLoaderSceneInfo m_MeshSceneInfo;

		Assimp::Importer* m_Importer;
	};
}