#include "pch.h"
#include "c_framebuffer.h"

Framebuffer::Framebuffer()
{

}

Framebuffer::~Framebuffer()
{
	m_renderTexture->Release();
	m_renderTextureView->Release();
	m_depthStencilTexture->Release();
	m_depthStencilView->Release();
}

void Framebuffer::initialize(ID3D11Device* device, int width, int height)
{
	m_width = width;
	m_height = height;
	D3D11_TEXTURE2D_DESC renderTextureDesc;
	ZeroMemory(&renderTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	renderTextureDesc.Width = width;
	renderTextureDesc.Height = height;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTextureDesc.SampleDesc.Count = 1;
	renderTextureDesc.SampleDesc.Quality = 0;
	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	HRESULT status = device->CreateTexture2D(&renderTextureDesc, NULL, &m_renderTexture);
	if (status != S_OK)
		LOGERROR("Failed to create framebuffer render texture!");

	status = device->CreateRenderTargetView(m_renderTexture, NULL, &m_renderTextureView);
	if (status != S_OK)
		LOGERROR("Failed to create framebuffer render texture view!");


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	status = device->CreateTexture2D(&depthStencilDesc, NULL, &m_depthStencilTexture);
	if (status != S_OK)
		LOGERROR("Failed to create framebuffer depthstencil texture!");


	status = device->CreateDepthStencilView(m_depthStencilTexture, NULL, &m_depthStencilView);
	if (status != S_OK)
		LOGERROR("Failed to create framebuffer depthstencil view!");
}