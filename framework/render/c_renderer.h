#pragma once
#include "pch.h"
#include "c_dx11renderer.h"
#include "c_modelmanager.h"
#include "c_texturemanager.h"
#include "c_materialmanager.h"
#include "c_shadermanager.h"
#include "c_camera.h"

class Renderer
{
public:
	Renderer();
	void initialize(HWND windowHandle, int width, int height);
	void beginRender();
	void endRender();
	Model* getModel(std::string name);
	Texture* getTexture(std::string name);
	Material* getMaterial(std::string name);
	void setCamera(Camera camera) { m_camera = camera; }
	void render(Model* model, Transform transform);
	void present();

private:
	Dx11Renderer m_dx11Renderer;
	ModelManager m_modelManager;
	TextureManager m_textureManager;
	MaterialManager m_materialManager;
	ShaderManager m_shaderManager;
	std::vector<Job*> m_jobVector;
	Camera m_camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 40.0f, 0.1f, 1000.0f, 16.0f / 9.0f);

	void proccessLoadJobs();
	void loadShader(std::string name);
};