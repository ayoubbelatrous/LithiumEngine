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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

		glGenTextures(1, &_EntityTexture);
		glBindTexture(GL_TEXTURE_2D, _EntityTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _EntityTexture, 0);
		
		glGenRenderbuffers(1, &_DepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _DepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _DepthBuffer); //
	
		

		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, DrawBuffers);

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
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glViewport(0, 0, width, height);
	}

	void FrameBuffer::BindTexture(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
	}

	unsigned int FrameBuffer::GetColorAttachmentID(int i) const
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
		/*
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, _EntityTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE,nullptr);
		*/
		glDeleteFramebuffers(1, &_id);
		glDeleteTextures(1, &_EntityTexture);
		glDeleteTextures(1, &renderedTexture);

		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		
		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
		

		glGenTextures(1, &_EntityTexture);
		glBindTexture(GL_TEXTURE_2D, _EntityTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _EntityTexture, 0);

		glGenRenderbuffers(1, &_DepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _DepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _DepthBuffer); //
		

		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1	};
		glDrawBuffers(1, DrawBuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int FrameBuffer::ReadPixel(int i, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	float FrameBuffer::ReadPixelFloat(int i, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		float pixelData;
		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixelData);
		return pixelData;
	}
	void FrameBuffer::ClearAttachment(int i,int value)
	{

		//*glClearTexImage(_EntityTexture, 0, GL_RED_INTEGER, GL_INT, &value);
		
		glClearBufferiv(_EntityTexture, 0, &value);
		
		

	}

}