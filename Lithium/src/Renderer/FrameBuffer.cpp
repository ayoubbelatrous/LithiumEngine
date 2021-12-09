#include "lipch.h"
#include "FrameBuffer.h"
#include "glad/glad.h"

namespace Lithium
{

	FrameBuffer::FrameBuffer()
	{
		width = 500;
		height = 500;

		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);


		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

		/*glGenTextures(1, &_EntityTexture);
		glBindTexture(GL_TEXTURE_2D, _EntityTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _EntityTexture, 0);*/

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		//glActiveTexture(GL_TEXTURE31);
		//glBindTexture(GL_TEXTURE_2D, renderedTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glViewport(0, 0, width, height);
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int FrameBuffer::GetColorAttachmentID() const
	{
		return renderedTexture;
	}
	void FrameBuffer::resize(int x, int y)
	{
		if (x <= 1 || y <= 1)
		{
			return;
		}
		width = x;
		height = y;
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	/*	glBindTexture(GL_TEXTURE_2D, _EntityTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);*/
	}

	int FrameBuffer::ReadPixel(int i, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void FrameBuffer::ClearAttachment(int i,int value)
	{
		//glClearTexImage(, 0,, GL_INT, &value);
		glClearTexImage(_EntityTexture, 0, GL_RED_INTEGER, GL_INT, &value);
		//glClearBufferiv(GL_COLOR, GL_COLOR_ATTACHMENT0, data);
	}

}