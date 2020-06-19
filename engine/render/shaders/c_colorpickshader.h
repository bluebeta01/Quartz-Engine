#pragma once
#include "render/c_shader.h"
#include <D3D11.h>

class ColorPickShader : public Shader
{
public:
	ID3D11Buffer* m_buffer = nullptr;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_renderTexture = nullptr;

	ColorPickShader(ID3D11Device* device);
	void screenResize(int width, int height);

private:
	ID3D11Device* m_device;
	void rebuildRenderTextures(int width, int height);
};