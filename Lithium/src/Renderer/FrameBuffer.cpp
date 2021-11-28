#include "FrameBuffer.h"
#include "Renderer.h"
#include <iostream>



FrameBuffer::FrameBuffer()
{
	width = 500;
	height = 500;
	
	GLCall(glGenFramebuffers(1,&_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _id));

	
	glGenTextures(1, &renderedTexture);
	GLCall(glActiveTexture(GL_TEXTURE31));
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0));
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, DrawBuffers)); // "1" is the size of DrawBuffers
	glBindTexture(GL_TEXTURE_2D, 0);

}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &_id);
}

unsigned int FrameBuffer::GetID() const
{
	return _id;
}

void FrameBuffer::Bind() const
{

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _id));
	GLCall(glViewport(0, 0, width, height));
}

void FrameBuffer::UnBind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int FrameBuffer::GetColorAttachmentID() const
{
	return renderedTexture;
}


void FrameBuffer::resize(int x,int y)
{
	if (x <= 1 || y<= 1)
	{
		return;
	}
	width = x;
	height = y;
	std::cout << "resized" << std::endl;

/*

	if (_id != 0)
	{
		glDeleteFramebuffers(1, &_id);
		glDeleteTextures(1, &renderedTexture);
	}
	_id = NULL;

	GLCall(glGenFramebuffers(1, &_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _id));


	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture*/
	GLCall(glActiveTexture(GL_TEXTURE31));
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
/*

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0));
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, DrawBuffers)); // "1" is the size of DrawBuffers*/

}
