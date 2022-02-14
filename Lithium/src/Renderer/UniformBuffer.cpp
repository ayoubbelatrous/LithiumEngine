#include "lipch.h"
#include "UniformBuffer.h"
#include <glad/glad.h>
UniformBuffer::UniformBuffer(uint32_t size,uint32_t binding)
{
	
	glGenBuffers(1, &_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, _Id);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, _Id);


}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &_Id);
}

void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
	glBindBuffer(GL_UNIFORM_BUFFER, _Id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

}

