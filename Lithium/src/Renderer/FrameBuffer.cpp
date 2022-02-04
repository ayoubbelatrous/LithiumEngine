#include "lipch.h"
#include "FrameBuffer.h"
#include "glad/glad.h"

namespace Lithium
{

	




	void FrameBuffer::ReCreate()
	{
	

		if (_id)
		{
			glDeleteFramebuffers(1, &_id);
			glDeleteTextures(textureIds.size(), textureIds.data());
			glDeleteTextures(1, &_DepthBuffer);
			textureIds.clear();
		}

		
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		for (size_t i = 0; i < descriptor.Attachments.size(); i++)
		{
			if (descriptor.Attachments[i].format == FramebufferTextureFormat::RGBA8)
			{
				
				textureIds.push_back(0);
				glGenTextures(1,&textureIds[i]);
				glBindTexture(GL_TEXTURE_2D, textureIds[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureIds[i], 0);
			}
			else if (descriptor.Attachments[i].format == FramebufferTextureFormat::Depth)
			{
				
				glGenRenderbuffers(1, &_DepthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, _DepthBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _DepthBuffer);
			}

			else if (descriptor.Attachments[i].format == FramebufferTextureFormat::RED_INTEGER)
			{
				textureIds.push_back(0);
				glGenTextures(1, &textureIds[i]);
				glBindTexture(GL_TEXTURE_2D, textureIds[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureIds[i], 0);

			}
		}

		GLenum drawbuffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };

		glDrawBuffers(textureIds.size(), drawbuffers);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::FrameBuffer(FrameBufferAttachmentDescriptor desc)
	{
		descriptor = desc;
		ReCreate();
	}
	FrameBuffer::FrameBuffer()
	{
		
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

	void FrameBuffer::BindTexture(uint32_t slot,int index) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, textureIds[index]);
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
	}

	unsigned int FrameBuffer::GetColorAttachmentID(int i) const
	{
		return textureIds[i];
		
	}
	void FrameBuffer::resize(int x, int y)
	{
		if (x <= 1 || y <= 1)
		{
			return;
		}
		width = x;
		height = y;
		
		ReCreate();

		
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