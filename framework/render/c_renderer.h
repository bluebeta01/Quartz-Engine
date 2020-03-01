#pragma once
#include "c_dx11renderer.h"
#include "c_modelmanager.h"
#include "c_texturemanager.h"
#include "c_materialmanager.h"
#include "c_shadermanager.h"
#include "c_camera.h"
#include "c_world.h"
#include "shaders/c_standardshader.h"
#include "shaders/c_colorpickshader.h"

class Renderer
{
public:
	int m_width;
	int m_height;
	StandardShader* m_standardShader = nullptr;
	ColorPickShader* m_colorPickShader = nullptr;

	Renderer(World* world);
	void initialize(HWND windowHandle, int width, int height);
	Model* getModel(std::string name);
	Texture* getTexture(std::string name);
	Material* getMaterial(std::string name);
	void setCamera(Camera camera) { m_camera = camera; }
	void render();
	void present();
	Entity* colorPick(glm::vec2 cursorPosition);

	glm::vec3 screenToWorldPosition(glm::vec2 cursorPosition);

	void proccessLoadJobs();
private:
	Dx11Renderer m_dx11Renderer;
	ModelManager m_modelManager;
	TextureManager m_textureManager;
	MaterialManager m_materialManager;
	ShaderManager m_shaderManager;
	std::vector<Job*> m_jobVector;
	Camera m_camera;
	World* m_world = nullptr;

	
};