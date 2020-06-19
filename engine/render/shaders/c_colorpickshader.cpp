#include "pch.h"
#include "c_colorpickshader.h"
#include "filesystem/filesystem.h"
#include "c_gamewindow.h"

ColorPickShader::ColorPickShader(ID3D11Device* device)
{
	m_device = device;

	std::string vertexShaderPath = filesystem::findFilePathByName("shaders/dx/colorpick.hlsl");
	std::string pixelShaderPath = filesystem::findFilePathByName("shaders/dx/colorpick.hlsl");

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
	UINT bufferSize = (sizeof(float) * 16 * 3) + (sizeof(float)*4);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = bufferSize + (16 - bufferSize);
	desc.StructureByteStride = 0;
	device->CreateBuffer(&desc, 0, &m_buffer);


	rebuildRenderTextures(GameWindow::s_clientSize.x, GameWindow::s_clientSize.y);
}

void ColorPickShader::rebuildRenderTextures(int width, int height)
{
	if(m_renderTexture)
		m_renderTexture->Release();
	if(m_renderTargetView)
		m_renderTargetView->Release();
	
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = GameWindow::s_clientSize.x;
	textureDesc.Height = GameWindow::s_clientSize.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	textureDesc.MiscFlags = 0;
	HRESULT status = m_device->CreateTexture2D(&textureDesc, nullptr, &m_renderTexture);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	status = m_device->CreateRenderTargetView(m_renderTexture, &renderTargetViewDesc, &m_renderTargetView);
}

void ColorPickShader::screenResize(int width, int height)
{
	rebuildRenderTextures(width, height);
}