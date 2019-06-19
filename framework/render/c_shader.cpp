#include "framework/pch.h"
#include "c_shader.h"
#include "framework/screen/screen.h"

Shader::Shader(std::string name, std::string vertexShaderFilePath, std::string pixelShaderFilePath)
{
	this->name = name;

	ID3D10Blob* VS, * PS, *VSErrors, *PSErrors;
	D3DX11CompileFromFile(vertexShaderFilePath.c_str(), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &VSErrors, 0);
	D3DX11CompileFromFile(pixelShaderFilePath.c_str(), 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &PSErrors, 0);

	if (VSErrors || PSErrors)
		LOGWARNING((char*)VSErrors->GetBufferPointer());

	screen::dxDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &dxVertexShader);
	screen::dxDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &dxPixelShader);


	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	screen::dxDevice->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &dxBufferLayout);
}