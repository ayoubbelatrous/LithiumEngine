#pragma once


namespace Lithium
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8,
	};
	struct FrameBufferTexture
	{
		FrameBufferTexture(FramebufferTextureFormat textureformat)
			: format(textureformat) {}

		FramebufferTextureFormat format;

	};
	struct FrameBufferAttachmentDescriptor
	{
		FrameBufferAttachmentDescriptor() = default;
		FrameBufferAttachmentDescriptor(std::initializer_list<FrameBufferTexture> attachments)
			: Attachments(attachments) {}
		std::vector<FrameBufferTexture> Attachments;

	};

	class FrameBuffer
	{
	private:
		unsigned int _id;
		unsigned int _rbid;
		int width;
		int height;
		unsigned int renderedTexture;
		unsigned int _EntityTexture;
		uint32_t _DepthBuffer;
		FrameBufferAttachmentDescriptor descriptor;
		void ReCreate();
		std::vector<uint32_t> textureIds;
	public:
		FrameBuffer();
		FrameBuffer(FrameBufferAttachmentDescriptor desc);
		~FrameBuffer();
		unsigned int GetID() const;
		void Bind() const;
		void BindTexture(uint32_t slot,int index) const;
		void UnBind() const;
		unsigned int GetColorAttachmentID(int i = 0) const;
		void resize(int x, int y);
		int ReadPixel(int i,int x,int y);
		float ReadPixelFloat(int i,int x,int y);
		void ClearAttachment(int i,int value);
	};
}