#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	
	GLCall(glGenBuffers(1, &_id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::VertexBuffer()
{
	_id = 0;
}

VertexBuffer::VertexBuffer(unsigned int size)
{
	GLCall(glGenBuffers(1, &_id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
}
void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &_id));
}


void VertexBuffer::SetData(void* data,unsigned int size) {
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}