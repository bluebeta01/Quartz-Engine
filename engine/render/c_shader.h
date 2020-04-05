#pragma once
#include <D3D11.h>

class Shader
{
public:
	virtual ~Shader() {};
	ID3D11VertexShader* m_dxVertexShader;
	ID3D11PixelShader* m_dxPixelShader;
	ID3D11InputLayout* m_dxBufferLayout;
private:
};