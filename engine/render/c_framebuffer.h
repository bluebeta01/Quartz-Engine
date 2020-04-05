#pragma once
#include <D3D11.h>

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	void initialize(ID3D11Device* device, int width, int height);
	ID3D11Texture2D* m_renderTexture;
	ID3D11RenderTargetView* m_renderTextureView;
	ID3D11Texture2D* m_depthStencilTexture;
	ID3D11DepthStencilView* m_depthStencilView;
	int m_width;
	int m_height;
};