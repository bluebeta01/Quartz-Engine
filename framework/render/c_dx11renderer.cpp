#include "pch.h"
#include "c_dx11renderer.h"

void Dx11Renderer::initialize(HWND windowHandle, int width, int height)
{
	m_width = width;
	m_height = height;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = windowHandle;                                // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG/*NULL*/,
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
	HRESULT r = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();



	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = 1280;
	depthStencilDesc.Height = 720;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT status = m_device->CreateTexture2D(&depthStencilDesc, NULL, &m_depthStencilBuffer);


	status = m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);


	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);


	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	status = m_device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);

	dsDesc.DepthEnable = false;
	status = m_device->CreateDepthStencilState(&dsDesc, &m_depthStencilStateNoDepth);
	



	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_device->CreateSamplerState(&sampDesc, &m_samplerState);


	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* rasterState;
	m_device->CreateRasterizerState(&rasterDesc, &rasterState);

	m_deviceContext->RSSetState(rasterState);
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
	m_deviceContext->ClearRenderTargetView(m_backBuffer, glm::value_ptr(glm::vec4(0, 0, 0, 1)));
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Dx11Renderer::uploadTexture(JobLoadTexture* job)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = job->textureData->width;
	desc.Height = job->textureData->height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = job->textureData->rawPixelData.data();
	initData.SysMemPitch = static_cast<UINT>(job->textureData->width * 4);
	initData.SysMemSlicePitch = static_cast<UINT>(job->textureData->width * job->textureData->height * 4);

	m_device->CreateTexture2D(&desc, &initData, &job->texture->m_dxTexture);


	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	memset(&SRVDesc, 0, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	m_device->CreateShaderResourceView(job->texture->m_dxTexture, &SRVDesc, &job->texture->m_dxResourceView);

}
void Dx11Renderer::clearFramebuffer(Framebuffer* framebuffer)
{
	m_deviceContext->ClearRenderTargetView(framebuffer->m_renderTextureView, glm::value_ptr(glm::vec4(0, 0.2, 0.4, 1)));
	m_deviceContext->ClearDepthStencilView(framebuffer->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Dx11Renderer::renderModel(Model* model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
	Material* overrideMaterial, StandardShader* shader, Framebuffer* frameBuffer)
{
	if (!model)
		return;

	if(frameBuffer)
		m_deviceContext->OMSetRenderTargets(1, &frameBuffer->m_renderTextureView, frameBuffer->m_depthStencilView);
	else
		m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	
	m_deviceContext->VSSetShader(shader->m_dxVertexShader, 0, 0);
	m_deviceContext->PSSetShader(shader->m_dxPixelShader, 0, 0);
	m_deviceContext->IASetInputLayout(shader->m_dxBufferLayout);

	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &model->dxVertexBuffer, &model->m_vertexSize, &offset);

	if(model->material)
		if(model->material->diffuseTexture)
			if(model->material->diffuseTexture->m_dxResourceView)
				m_deviceContext->PSSetShaderResources(0, 1, &model->material->diffuseTexture->m_dxResourceView);
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	projectionMatrix = glm::transpose(projectionMatrix);
	modelMatrix = glm::transpose(modelMatrix);
	viewMatrix = glm::transpose(viewMatrix);

	glm::mat4 cb[3] = { modelMatrix, viewMatrix, projectionMatrix };

	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(shader->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, cb, sizeof(float)*16*3);
	m_deviceContext->Unmap(shader->m_matrixBuffer, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &shader->m_matrixBuffer);


	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw(model->vertCount, 0);
}
void Dx11Renderer::enableDepth(bool enable)
{
	if (enable)
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
	}
	else
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilStateNoDepth, 0);
	}
}
void Dx11Renderer::present()
{
	m_swapChain->Present(1, 0);
}
void Dx11Renderer::clearColorPick(ColorPickShader* shader)
{
	m_deviceContext->ClearRenderTargetView(shader->m_renderTargetView, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Dx11Renderer::renderColorPickModel(Model* model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, ColorPickShader* shader, glm::vec3 color)
{
	if (!model)
		return;

	m_deviceContext->OMSetRenderTargets(1, &shader->m_renderTargetView, m_depthStencilView);
	m_deviceContext->VSSetShader(shader->m_dxVertexShader, 0, 0);
	m_deviceContext->PSSetShader(shader->m_dxPixelShader, 0, 0);
	m_deviceContext->IASetInputLayout(shader->m_dxBufferLayout);

	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &model->dxVertexBuffer, &model->m_vertexSize, &offset);

	projectionMatrix = glm::transpose(projectionMatrix);
	modelMatrix = glm::transpose(modelMatrix);
	viewMatrix = glm::transpose(viewMatrix);

	//glm::mat4 cb[3] = { modelMatrix, viewMatrix, projectionMatrix };

	struct bufferData
	{
		glm::mat4 mm, vm, pm;
		glm::vec4 color;
	}data;
	data.mm = modelMatrix;
	data.vm = viewMatrix;
	data.pm = projectionMatrix;
	data.color = glm::vec4(color, 1.0f);

	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(shader->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &data, sizeof(bufferData));
	m_deviceContext->Unmap(shader->m_buffer, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &shader->m_buffer);


	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw(model->vertCount, 0);
}
float roundTwoPlaces(float value)
{
	float v = (int)(value * 100 + 0.5);
	return (float)v / 100.0f;
}
glm::vec3 Dx11Renderer::getColorPickColor(glm::vec2 cursorPosition, ColorPickShader* shader)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;
	ID3D11Texture2D* texture;
	HRESULT status = m_device->CreateTexture2D(&desc, nullptr, &texture);
	
	ID3D11Resource* resource;
	m_backBuffer->GetResource(&resource);
	m_deviceContext->CopyResource(texture, shader->m_renderTexture);



	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &ms);
	uint16_t* data = (uint16_t*)malloc(ms.DepthPitch);
	memcpy(data, ms.pData, ms.DepthPitch);
	m_deviceContext->Unmap(texture, 0);


	UINT yOffset = cursorPosition.y * m_width * 4;
	UINT xOffset = cursorPosition.x * 4;
	UINT offset = xOffset + yOffset;

	uint16_t pixelColor[4];
	memcpy(pixelColor, &data[offset], sizeof(uint16_t)*4);
	
	float r = roundTwoPlaces(((float)(uint16_t)pixelColor[0]) / 65535.0);
	float g = roundTwoPlaces(((float)(uint16_t)pixelColor[1]) / 65535.0);
	float b = roundTwoPlaces(((float)(uint16_t)pixelColor[2]) / 65535.0);

	texture->Release();
	resource->Release();
	delete data;

	return glm::vec3(r,g,b);
}


glm::vec3 Dx11Renderer::screenToWorldPosition(glm::vec2 cursorPosition, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Framebuffer* framebuffer)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;
	ID3D11Texture2D* texture;
	HRESULT status = m_device->CreateTexture2D(&desc, nullptr, &texture);

	if(framebuffer)
		m_deviceContext->CopyResource(texture, framebuffer->m_depthStencilTexture);
	else
		m_deviceContext->CopyResource(texture, m_depthStencilBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &ms);
	UINT* data = (UINT*)malloc(ms.DepthPitch);
	memcpy(data, ms.pData, ms.DepthPitch);
	m_deviceContext->Unmap(texture, 0);

	UINT yOffset = cursorPosition.y * m_width;
	UINT xOffset = cursorPosition.x;
	UINT offset = yOffset + xOffset;

	int dataElementCount = ms.DepthPitch / 4;
	if (offset > dataElementCount)
		offset = 0;

	UINT result = data[offset];
	result = result >> 8;
	float depth = result / 65535.0f;

	texture->Release();
	delete data;

	glm::mat4 m = projectionMatrix * viewMatrix;
	m = glm::inverse(m);

	float mouseX = (2.0f * (cursorPosition.x / 1280.0f)) - 1.0f;
	float mouseY = -1 * ((2.0f * (cursorPosition.y / 720.0f)) - 1.0f);

	glm::vec4 screenPos = { mouseX, mouseY, depth, 1.0f };
	glm::vec4 worldPos = m * screenPos;

	worldPos.w = 1.0f / worldPos.w;
	worldPos.x *= worldPos.w;
	worldPos.y *= worldPos.w;
	worldPos.z *= worldPos.w;

	return worldPos;
}