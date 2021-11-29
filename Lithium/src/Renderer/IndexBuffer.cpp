#include "lipch.h"
#include <glad/glad.h>
#include "IndexBuffer.h"

namespace Lithium
{

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		:_count(count)
	{
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::IndexBuffer()
	{
		_id = 0;
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}
	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

}