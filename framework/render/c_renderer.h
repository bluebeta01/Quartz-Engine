#pragma once
#include "c_dx11renderer.h"
#include "c_modelmanager.h"
#include "c_texturemanager.h"
#include "c_materialmanager.h"
#include "c_shadermanager.h"
#include "c_camera.h"
#include "c_world.h"

class Renderer
{
public:
	int m_width;
	int m_height;

	Renderer(World* world);
	void initialize(HWND windowHandle, int width, int height);
	void beginRender();
	void endRender();
	Model* getModel(std::string name);
	Texture* getTexture(std::string name);
	Material* getMaterial(std::string name);
	void setCamera(Camera camera) { m_camera = camera; }
	void render();
	void present();

private:
	Dx11Renderer m_dx11Renderer;
	ModelManager m_modelManager;
	TextureManager m_textureManager;
	MaterialManager m_materialManager;
	ShaderManager m_shaderManager;
	std::vector<Job*> m_jobVector;
	Camera m_camera;
	World* m_world = nullptr;

	void proccessLoadJobs();
	void loadShader(std::string name);
};