#pragma once
#include "pch.h"
#include "jobs.h"
#include "c_shader.h"
#include "shaders/c_standardshader.h"
#include "shaders/c_colorpickshader.h"
#include "c_framebuffer.h"
#include "c_gamewindow.h"

class Dx11Renderer
{
public:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	glm::vec3 getColorPickColor(glm::vec2 cursorPosition, ColorPickShader* shader);
	glm::vec3 screenToWorldPosition(glm::vec2 cursorPosition, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Framebuffer* framebuffer);
	void initialize();
	void uploadModel(JobLoadModel* job);
	void uploadTexture(JobLoadTexture* job);
	void renderModel(Model* model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
		Material* overrideMaterial, StandardShader* shader, Framebuffer* frameBuffer);
	void clearColorPick(ColorPickShader* shader);
	void renderColorPickModel(Model* model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, ColorPickShader* shader, glm::vec3 color);
	void enableDepth(bool);
	void clearFramebuffer(Framebuffer* framebuffer);
	void onResize();
	void clearFrame();
	void present();
	void resetRenderTarget();
	void resetViewport();
	void setCustomViewport(int width, int height);

private:
	ID3D11SamplerState* m_samplerState;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_backBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthStencilStateNoDepth;
	void rebuildDx();
};