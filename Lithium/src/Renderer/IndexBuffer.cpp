#include "lipch.h"

#include "IndexBuffer.h"
#include <glad/glad.h>



namespace Lithium
{

	IndexBuffer::IndexBuffer(uint32_t size)
	{
		glGenBuffers(1, &_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,nullptr,GL_DYNAMIC_DRAW);
	}

	IndexBuffer::IndexBuffer(uint32_t size, const void* data)
	{
		glGenBuffers(1, &_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &_Id);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _Id);
	}

	void IndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	void IndexBuffer::SetData(uint32_t size, const void* data)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

	}

}