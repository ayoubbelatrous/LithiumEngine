#pragma once


namespace Lithium
{
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
	public:
		FrameBuffer();
		~FrameBuffer();
		unsigned int GetID() const;
		void Bind() const;
		void BindTexture(uint32_t slot) const;
		void UnBind() const;
		unsigned int GetColorAttachmentID(int i = 0) const;
		void resize(int x, int y);
		int ReadPixel(int i,int x,int y);
		float ReadPixelFloat(int i,int x,int y);
		void ClearAttachment(int i,int value);
	};
}