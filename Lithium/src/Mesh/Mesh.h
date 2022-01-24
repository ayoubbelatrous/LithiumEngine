#pragma once
#include "glm.hpp"
#include <vector>
#include <Core/Base.h>
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"


namespace Lithium
{
	struct MeshVertex
	{
		glm::vec3 pos;
	};
	class Mesh
	{
	private:
		MeshVertex* _Vertices = nullptr;
		std::vector<glm::vec3> _Uvs;
		std::vector<glm::vec3> _Indices;
		std::vector<glm::vec3> _Normals;
		Ref<VertexBuffer> _VertexBuffer;
		Ref<IndexBuffer> _IndexBuffer;
		Ref<VertexArray> _VertexArray;
		int count;
	public:
	
		
		void setVertices(std::vector<glm::vec3> verts)
		{
			uint32_t size = verts.size();
			_Vertices = new MeshVertex[size];
			for (glm::vec3 vertex:verts)
			{
				
				_Vertices->pos =  vertex;
				_Vertices++;
			}
			count = size;
			
		}

		void Init();

		void Draw();
	};
	
}

namespace MeshLoader
{
	std::vector<glm::vec3> LoadModel(const std::string path);
}
