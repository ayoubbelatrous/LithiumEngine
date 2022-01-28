#include "lipch.h"
#include "Mesh/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Renderer.h"

namespace Lithium
{
	Mesh::Mesh()
	{

	}
	Mesh::~Mesh()
	{

	}

	Ref<Mesh> Mesh::LoadMesh(const std::string& path)
	{
		std::vector<MeshVertex> vertices;
		std::vector<uint32_t> indices;

		Ref<Mesh> _mesh = CreateRef<Mesh>();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


		
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CORE_LOG("ERROR::MESHLOADER::" << importer.GetErrorString());
			return _mesh;
		}

		aiMesh* mesh = scene->mMeshes[0];
		if (!mesh)
		{
			CORE_LOG("ERROR::MESHLOADER:  NO MESHES FOUND IN FILE");
			return _mesh;
		}
			
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			MeshVertex vertex;
			vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertices.push_back(vertex);
		}
		if (mesh->HasNormals())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				vertices[i].normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
		}
		else {
			CORE_LOG("ERROR::MESHLOADER:  MESH HAS NO NORMALS");
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}

		}


		_mesh->SetVertices(vertices);
		_mesh->SetIndices(indices);
		_mesh->SetupMesh();
		return _mesh;
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
		_VertexArray->AddBuffer(_VertexBuffer, layout);

	}

	void Mesh::Render()
	{
		_VertexArray->Bind();
		_IndexBuffer->Bind();
		RendererCommand::DrawIndexed(_Indices.size());
	}
}
