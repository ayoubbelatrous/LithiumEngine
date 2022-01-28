#include "lipch.h"
#include "glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Core/Base.h"
#include <Renderer/VertexBuffer.h>
#include <Renderer/IndexBuffer.h>
#include <Renderer/VertexArray.h>
#include "Renderer/Renderer.h"
#include "Mesh.h"

std::vector<unsigned int> indices;
std::vector<glm::vec3> verts;
std::vector<glm::vec3> normals;

namespace MeshLoader
{
	
	std::vector<glm::vec3> LoadModel(const std::string path)
	{
		std::vector<glm::vec3> _Verts;

		const aiScene* scene;
		Assimp::Importer importer;
		scene = importer.ReadFile(path.c_str(), 0);
		CORE_LOG("loading Mesh");
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CORE_LOG("ERROR::ASSIMP::" << importer.GetErrorString());
			return _Verts;
		}
		
		aiMesh* mesh = scene->mMeshes[0];
		if (!mesh)
			return _Verts;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			_Verts.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));

		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
				
			}

		}



		verts = _Verts;
		return _Verts;
	}



}

/*float cube_vertices[] = {
				// front
				-1.0, -1.0,  1.0,
				 1.0, -1.0,  1.0,
				 1.0,  1.0,  1.0,
				-1.0,  1.0,  1.0,
				// back
				-1.0, -1.0, -1.0,
				 1.0, -1.0, -1.0,
				 1.0,  1.0, -1.0,
				-1.0,  1.0, -1.0
			};
			unsigned int cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// right
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// left
				4, 0, 3,
				3, 7, 4,
				// bottom
				4, 5, 1,
				1, 0, 4,
				// top
				3, 2, 6,
				6, 7, 3
			};
			_VertexArray = CreateRef<VertexArray>();
			_VertexBuffer = CreateRef<VertexBuffer>(cube_vertices, sizeof(cube_vertices));
			_IndexBuffer = CreateRef<IndexBuffer>(sizeof(cube_elements), (void*)&cube_elements);

			Ref<VertexBufferLayout> layout =
				CreateRef<VertexBufferLayout>();
			layout->Push<float>(3);
			_VertexArray->AddBuffer(_VertexBuffer, layout);*/
namespace Lithium
{

		void Mesh::Init()

		{
			/*
			float cube_vertices[] = {
				// front
				-1.0, -1.0,  1.0,
				 1.0, -1.0,  1.0,
				 1.0,  1.0,  1.0,
				-1.0,  1.0,  1.0,
				// back
				-1.0, -1.0, -1.0,
				 1.0, -1.0, -1.0,
				 1.0,  1.0, -1.0,
				-1.0,  1.0, -1.0
			};
			unsigned int cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// right
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// left
				4, 0, 3,
				3, 7, 4,
				// bottom
				4, 5, 1,
				1, 0, 4,
				// top
				3, 2, 6,
				6, 7, 3
			};
			*/
			

			for (size_t i = 0; i < normals.size(); i++)
			{
				_Vertices[i].normal = normals[i];
			}
			_VertexArray = CreateRef<VertexArray>();
			_VertexBuffer = CreateRef<VertexBuffer>(&_Vertices[0], sizeof(MeshVertex) * count);
			_IndexBuffer = CreateRef<IndexBuffer>(sizeof(indices) * 4,&indices[0]);
			for (auto i : indices)
			{
				CORE_LOG(i);
			}

		
			
			//CORE_LOG("vert size :" << sizeof(indices));
			Ref<VertexBufferLayout> layout =
				CreateRef<VertexBufferLayout>();
			layout->Push<float>(3);
			layout->Push<float>(3);
			_VertexArray->AddBuffer(_VertexBuffer, layout);
		}

		void Mesh::Draw()
		{
			_VertexArray->Bind();
			_VertexBuffer->Bind();
			_IndexBuffer->Bind();
			RendererCommand::DrawIndexed(55);
		}

}
