#pragma once
#include "pch.h"

class Shader
{
public:
	Shader(std::string name, std::string vertexShaderFilePath, std::string pixelShaderFilePath, ID3D11Device* device);
	std::string name;
	ID3D11VertexShader* dxVertexShader;
	ID3D11PixelShader* dxPixelShader;
	ID3D11InputLayout* dxBufferLayout;
private:
};