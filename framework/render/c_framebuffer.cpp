#include "c_framebuffer.h"
#include "GL/glew.h"

FrameBuffer::FrameBuffer(int sizeX, int sizeY)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenFramebuffers(1, &glFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, glFrameBufferId);

	//Generate diffuse texture and attach it
	glGenTextures(1, &glFrameBufferColor);
	glBindTexture(GL_TEXTURE_2D, glFrameBufferColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glFrameBufferColor, 0);

	//Generate render buffer texture and attach it
	glGenRenderbuffers(1, &glFrameBufferRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, glFrameBufferRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glFrameBufferRenderBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &glFrameBufferId);
	glDeleteTextures(1, &glFrameBufferColor);
	glDeleteRenderbuffers(1, &glFrameBufferRenderBuffer);
}