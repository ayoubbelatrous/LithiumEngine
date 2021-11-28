#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:_count(count)
{
	
	GLCall(glGenBuffers(1, &_id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
	
}

IndexBuffer::IndexBuffer()
{
	_id = 0;
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
}
void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &_id));
}
