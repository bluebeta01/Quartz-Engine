#pragma once
#include "framework/pch.h"

class FrameBuffer
{
public:
	unsigned int glFrameBufferId;
	unsigned int glFrameBufferColor;
	unsigned int glFrameBufferRenderBuffer;

	FrameBuffer(int sizeX, int sizeY);
	~FrameBuffer();
};