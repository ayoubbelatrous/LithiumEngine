#include "lipch.h"

#include "VertexBufferLayout.h"
#include <glad/glad.h>

namespace Lithium
{
	unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
	{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			case GL_INT: return 4;
			}
			return 0;
	}

	template<typename T>
	void Push(unsigned int count)
	{
	}
	template<>
	void VertexBufferLayout::Push<float>(unsigned int count)
	{
		elements.push_back({ count,GL_FLOAT,false });
		Stride += 4 * count;
	}

	template<>
	void VertexBufferLayout::Push<unsigned int>(unsigned int count)
	{
		elements.push_back({ count,GL_UNSIGNED_INT,false });
		Stride += 4 * count;
	}

	template<>
	void VertexBufferLayout::Push<char>(unsigned int count)
	{
		elements.push_back({ count,GL_UNSIGNED_BYTE,true });
		Stride += 1 * count;
	}

	template<>
	void VertexBufferLayout::Push<int>(unsigned int count)
	{
		elements.push_back({ count,GL_INT,false });
		Stride += 4 * count;
	}
	
}