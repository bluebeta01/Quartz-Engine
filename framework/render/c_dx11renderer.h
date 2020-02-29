#pragma once
#include "pch.h"
#include "jobs.h"
#include "c_shader.h"

class Dx11Renderer
{
public:
	void initialize(HWND windowHandle, int width, int height);
	void uploadModel(JobLoadModel* job);
	void uploadTexture(JobLoadTexture* job);
	void renderModel(Model* model, Transform transform, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Material* overrideMaterial);
	void clearFrame();
	void bindShader(Shader* shader);
	void present();

	ID3D11Device* m_device;
private:
	IDXGISwapChain* m_swapChain;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_backBuffer;
};