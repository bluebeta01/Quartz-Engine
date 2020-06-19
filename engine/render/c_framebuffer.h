#pragma once
#include <D3D11.h>

class Framebuffer
{
public:
	int m_width;
	int m_height;
	ID3D11Device* m_device;
	ID3D11Texture2D* m_renderTexture;
	ID3D11RenderTargetView* m_renderTextureView;
	ID3D11Texture2D* m_depthStencilTexture;
	ID3D11DepthStencilView* m_depthStencilView;

	Framebuffer() {};
	~Framebuffer();
	void initialize(ID3D11Device* device, int width, int height);
	void rebuildFramebuffer(int width, int height);
};