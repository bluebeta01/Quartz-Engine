#include "pch.h"
#include "c_dx11renderer.h"

void Dx11Renderer::initialize(HWND windowHandle, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = windowHandle;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext);

	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	m_deviceContext->RSSetViewports(1, &viewport);
}
void Dx11Renderer::uploadModel(JobLoadModel* job)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(float) * job->modelData->vertCount * job->modelData->floatsPerVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_device->CreateBuffer(&bd, NULL, &job->model->dxVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(job->model->dxVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, job->modelData->data, sizeof(float) * job->modelData->vertCount * job->modelData->floatsPerVert);
	m_deviceContext->Unmap(job->model->dxVertexBuffer, NULL);
}
void Dx11Renderer::clearFrame()
{
	m_deviceContext->ClearRenderTargetView(m_backBuffer, glm::value_ptr(glm::vec4(0, 0.2, 0.4, 1)));
}
void Dx11Renderer::bindShader(Shader* shader)
{
	m_deviceContext->VSSetShader(shader->dxVertexShader, 0, 0);
	m_deviceContext->PSSetShader(shader->dxPixelShader, 0, 0);
	m_deviceContext->IASetInputLayout(shader->dxBufferLayout);
}
void Dx11Renderer::uploadTexture(JobLoadTexture* job)
{

}
void Dx11Renderer::renderModel(Model* model, Transform transform, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Material* overrideMaterial)
{
	UINT stride = sizeof(float) * 8;
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &model->dxVertexBuffer, &stride, &offset);

	glm::mat4 mm = Transform::matrixFromTransform(transform, true);
	glm::mat4 cb[3] = { mm, viewMatrix, projectionMatrix };
	UINT cbSize = sizeof(float) * 16 * 3;

	ID3D11Buffer* constantBuffer;
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = cbSize + (16 - cbSize);
	desc.StructureByteStride = 0;
	m_device->CreateBuffer(&desc, 0, &constantBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, cb, cbSize);
	m_deviceContext->Unmap(constantBuffer, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	constantBuffer->Release();

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw(model->vertCount, 0);
}
void Dx11Renderer::present()
{
	m_swapChain->Present(0, 0);
}