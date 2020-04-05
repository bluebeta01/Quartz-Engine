#include "pch.h"
#include "c_standardshader.h"
#include "filesystem/filesystem.h"

StandardShader::StandardShader(ID3D11Device* device)
{
	std::string vertexShaderPath = filesystem::findFilePathByName("shaders/dx/standard.hlsl");
	std::string pixelShaderPath = filesystem::findFilePathByName("shaders/dx/standard.hlsl");

	ID3D10Blob* VS, * PS, * VSErrors, * PSErrors;
	D3DX11CompileFromFile(vertexShaderPath.c_str(), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &VSErrors, 0);
	D3DX11CompileFromFile(pixelShaderPath.c_str(), 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &PSErrors, 0);

	if (VSErrors || PSErrors)
		LOGWARNING((char*)VSErrors->GetBufferPointer());

	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_dxVertexShader);
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_dxPixelShader);


	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	device->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &m_dxBufferLayout);


	//Setup matrix buffer
	UINT bufferSize = sizeof(float) * 16 * 3;

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = bufferSize + (16 - bufferSize);
	desc.StructureByteStride = 0;
	device->CreateBuffer(&desc, 0, &m_matrixBuffer);
}

StandardShader::~StandardShader()
{

}