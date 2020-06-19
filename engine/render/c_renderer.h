#pragma once
#include "c_dx11renderer.h"
#include "c_modelmanager.h"
#include "c_texturemanager.h"
#include "c_materialmanager.h"
#include "c_camera.h"
#include "c_world.h"
#include "shaders/c_standardshader.h"
#include "shaders/c_colorpickshader.h"
#include "c_framebuffer.h"

class Renderer
{
public:
	Renderer(World* world);
	glm::vec3 screenToWorldPosition(glm::vec2 cursorPosition, Framebuffer* framebuffer);
	void initialize();
	void setCamera(Camera camera) { m_currentCamera = camera; }
	void render();
	void present();
	void renderModel(Model* model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
		Material* overrideMaterial, Framebuffer* frameBuffer, bool depth);
	void clearFramebuffer(Framebuffer* framebuffer);
	void proccessLoadJobs();
	void tick();
	StandardShader* m_standardShader = nullptr;
	ColorPickShader* m_colorPickShader = nullptr;
	Model* getModel(std::string name);
	Texture* getTexture(std::string name);
	Material* getMaterial(std::string name);
	Entity* colorPick(glm::vec2 cursorPosition);
	Framebuffer* createFramebuffer(int width, int height);

private:
	void onResize();

	std::vector<Job*> m_jobVector;
	Dx11Renderer m_dx11Renderer;
	ModelManager m_modelManager;
	TextureManager m_textureManager;
	MaterialManager m_materialManager;
	Camera m_currentCamera;
	World* m_world;
};