#include "lipch.h"

#include "VertexBuffer.h"
#include <glad/glad.h>


namespace Lithium
{
	
	VertexBuffer::VertexBuffer(uint32_t size)
	{
		glGenBuffers(1, &_Id);
		glBindBuffer(GL_ARRAY_BUFFER, _Id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	{
		glGenBuffers(1, &_Id);
		glBindBuffer(GL_ARRAY_BUFFER, _Id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0,size, data);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _Id);
	}

	void VertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &_Id);
	}
	uint32_t VertexBuffer::GetID()
	{
		return _Id;
	}
}

