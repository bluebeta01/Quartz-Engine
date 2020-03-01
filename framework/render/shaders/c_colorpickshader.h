#pragma once
#include "render/c_shader.h"
#include <D3D11.h>

class ColorPickShader : public Shader
{
public:
	ColorPickShader(ID3D11Device* device);
	ID3D11Buffer* m_buffer = nullptr;
	ID3D11Texture2D* m_renderTexture = nullptr;
	ID3D11RenderTargetView* m_renderTargetView;
};