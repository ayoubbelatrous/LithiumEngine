#include "lipch.h"
#include "SceneRenderer/Mesh/MeshLoader.h"
#include "Core/Log.h"

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>
#include <assimp/postprocess.h>




namespace Lithium::Rendering::Loaders
{

	MeshLoader::MeshLoader(const std::string& path)
	{
		m_Path = path;
		//m_LoadThread = std::thread(&MeshLoader::Load,this);
		Load();
	}

	MeshLoader::~MeshLoader() {
		m_LoadThread.join();
	}

	std::vector<glm::vec3> MeshLoader::GetVertices() const
	{
		return m_Vertices;
	}

	std::vector<uint32_t> MeshLoader::GetIndices() const
	{
		return m_Indices;
	}

	std::vector<glm::vec4> MeshLoader::GetColors() const
	{
		return m_Colors;
	}

	std::vector<glm::vec2> MeshLoader::GetTextureCoordinates() const
	{
		return m_TextureCoordinates;
	}

	void MeshLoader::Load()
	{

		m_Importer = new Assimp::Importer;

		const aiScene* scene = m_Importer->ReadFile(m_Path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		if (scene == nullptr)
		{
			LT_CORE_ERROR("MeshLoader :: An error has occured while loading mesh :: {0}", m_Importer->GetErrorString());
			return;
		}

		m_MeshSceneInfo = {};
	}

	void MeshLoader::LoadVertices()
	{

	}

	void MeshLoader::LoadIndices()
	{

	}

	void MeshLoader::LoadColors()
	{

	}

	void MeshLoader::LoadTextureCoordinates()
	{

	}

	bool MeshLoader::LoadFinished()
	{
		return m_FinishedLoading;
	}

}