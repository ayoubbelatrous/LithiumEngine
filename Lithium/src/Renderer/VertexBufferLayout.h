#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{

	unsigned int count;
	unsigned int type;
	bool normalize;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false)
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int Stride;
public:

	VertexBufferLayout()
		:Stride(0) {}
	~VertexBufferLayout(){}


	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		elements.push_back({ count,GL_FLOAT,false });
		Stride += 4 * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		elements.push_back({ count,GL_UNSIGNED_INT,false });
		Stride += 4 * count;
	}

	template<>
	void Push<char>(unsigned int count)
	{
		elements.push_back({ count,GL_UNSIGNED_BYTE,true });
		Stride += 1 * count;
	}


	inline unsigned int GetStride() const { return Stride; }
	inline std::vector<VertexBufferElement> GetElements() const { return elements; }
};
