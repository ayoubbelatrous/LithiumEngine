#include "lipch.h"
#include <glad/glad.h>
#include "VertexBuffer.h"

namespace Lithium
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{

		glGenBuffers(1, &_id);
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer()
	{
		_id = 0;
	}

	VertexBuffer::VertexBuffer(unsigned int size)
	{
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}
	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}


	void VertexBuffer::SetData(void* data, unsigned int size) {
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}
}