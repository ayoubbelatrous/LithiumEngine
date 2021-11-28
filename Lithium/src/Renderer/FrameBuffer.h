#pragma once


class FrameBuffer
{
private:
	unsigned int _id;
	unsigned int _rbid;
	int width;
	int height;
	unsigned int renderedTexture;
public:
	FrameBuffer();
	~FrameBuffer();
	unsigned int GetID() const;
	void Bind() const;
	void UnBind() const;
	unsigned int GetColorAttachmentID() const;
	void resize(int x,int y);
};