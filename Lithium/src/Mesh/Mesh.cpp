#include "lipch.h"
#include "Mesh/Mesh.h"


#include "Renderer/Renderer.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <mutex>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Lithium
{
	static Assimp::Importer importer;

	static std::vector<Ref<Mesh>> _MeshList;
	Mesh::Mesh()
	{
	}
	Mesh::~Mesh()
	{

	}
	static Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<MeshVertex> vertices;
		std::vector<uint32_t> indices; 
		Ref<Mesh> _Mesh;


		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			MeshVertex vertex;
			glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.position = position;
			vertices.push_back(vertex);
		}

		if (mesh->HasNormals())
		{
			for (size_t i = 0; i < mesh->mNumVertices; i++)
			{
				glm::vec3 normal(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				;
				vertices[i].normal = normal;
				
			}
		}
		if (mesh->mTextureCoords[0])
		{
			for (size_t i = 0; i < mesh->mNumVertices; i++)
			{
				glm::vec2 uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				vertices[i].texcoords = uv;
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		_Mesh = CreateRef<Mesh>();
		_Mesh->SetVertices(vertices);
		_Mesh->SetIndices(indices);
		_Mesh->SetupMesh();
		return _Mesh;
	}
	static std::vector<Ref<Mesh>> ProcessNode(aiNode* node, const aiScene* scene)
	{
		

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_MeshList.push_back(ProcessMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}

		return _MeshList;
	}

	std::vector<Ref<Mesh>> Mesh::LoadMesh(const std::string& path)
	{
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate| aiProcess_GenSmoothNormals);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			CORE_LOG( "ERROR::ASSIMP:: " << importer.GetErrorString());
			std::vector<Ref<Mesh>> mesh;
			return mesh;
		}
		return ProcessNode(scene->mRootNode, scene);
		
	}


	void Mesh::SetVertices(const std::vector<MeshVertex>& vertices)
	{
		_Vertices = vertices;
	}
	void Mesh::SetIndices(const std::vector<uint32_t>& indices)
	{
		_Indices = indices;

	}

	void Mesh::SetupMesh()
	{
		_VertexArray = CreateRef<VertexArray>();
		_VertexBuffer = CreateRef<VertexBuffer>(&_Vertices[0],sizeof(MeshVertex) * _Vertices.size());
		_IndexBuffer = CreateRef<IndexBuffer>(sizeof(uint32_t) * _Indices.size(),&_Indices[0]);
		Ref<VertexBufferLayout> layout = CreateRef<VertexBufferLayout>();

		layout->Push<float>(3);
		layout->Push<float>(3);
		layout->Push<float>(2);
		_VertexArray->AddBuffer(_VertexBuffer, layout);

	}

	void Mesh::Render()
	{
		_VertexArray->Bind();
		_IndexBuffer->Bind();
		RendererCommand::DrawIndexed(_Indices.size());
	}
}
