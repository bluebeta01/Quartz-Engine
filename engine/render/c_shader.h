#pragma once
#include <D3D11.h>

class Shader
{
public:
	ID3D11VertexShader* m_dxVertexShader;
	ID3D11PixelShader* m_dxPixelShader;
	ID3D11InputLayout* m_dxBufferLayout;

	virtual ~Shader() {};
	virtual void screenResize(int width, int height) {};
private:
};