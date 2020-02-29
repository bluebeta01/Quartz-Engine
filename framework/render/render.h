/*#pragma once
#include "c_model.h"
#include "c_texture.h"
#include "c_material.h"
#include "c_pointlight.h"
#include "framework/common/entity/c_entity.h"
#include "framework/pch.h"
#include "c_camera.h"
#include "c_framebuffer.h"
#include "c_shader.h"
#include "c_shadermanager.h"
#include "jobs.h"

struct SHADER_CONSTANT_BUFFER
{
	unsigned int slotValue;
	unsigned int bufferDataSize;
	void* data;
};

class Renderer
{
public:
	ShaderManager shaderManager;
	Renderer(int viewportSizeX, int viewportSizeY);
	~Renderer();
	void prepFrame();
	Model* getModel(std::string name);
	Texture* getTexture(std::string name);
	Material* getMaterial(std::string name);
	void setProjectionMatrix(glm::mat4 matrix);
	void setViewMatrix(glm::mat4 matrix);
	void setViewportSize(int x, int y);
	void drawEntities(std::vector<Entity*> entities);
	void setLightsVector(std::vector<PointLight> pointLights);
	void setFrameBuffer(FrameBuffer* frameBuffer);

	void renderModel(Model* model, Shader* shader, unsigned int shaderConstantBufferCount, SHADER_CONSTANT_BUFFER* shaderConstantBufferArray);
	glm::mat4 matrixFromTransform(Transform transform, bool correctRotation);


	void clearFrame();

	void processRenderJobs();
private:
	int viewportSizeX, viewportSizeY;
	void loadShader(std::string name);
	void parseModel(JobLoadModel* jobLoadModel);
};*/