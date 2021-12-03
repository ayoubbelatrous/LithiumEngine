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
		//glActiveTexture(GL_TEXTURE31);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
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
		//glActiveTexture(GL_TEXTURE17);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}
}