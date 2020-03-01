#include "pch.h"
#include "c_renderer.h"
#include "jobs.h"
#include "asset/asset.h"
#include "filesystem/filesystem.h"

Renderer::Renderer(World* world) : m_world(world)
{

}

void Renderer::initialize(HWND windowHandle, int width, int height)
{
	m_width = width;
	m_height = height;
	m_camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 40.0f, 0.1f, 1000.0f, (float)m_width / (float)m_height);
	m_dx11Renderer.initialize(windowHandle, width, height);
	m_standardShader = new StandardShader(m_dx11Renderer.m_device);
	m_colorPickShader = new ColorPickShader(m_dx11Renderer.m_device);
}

Model* Renderer::getModel(std::string name)
{
	Model* m = m_modelManager.getModelByName(name);
	if (m)
		return m;

	Model* model = new Model();
	JobLoadModel* jobLoadModel = new JobLoadModel();
	jobLoadModel->nameOfModelToLoad = name;
	jobLoadModel->model = model;
	jobLoadModel->modelData = asset::assetLoadModelData(name);
	m_jobVector.push_back(jobLoadModel);
	return model;
}

Texture* Renderer::getTexture(std::string name)
{
	Texture* t = m_textureManager.getTextureByName(name);
	if (t)
		return t;

	Texture* texture = new Texture();
	texture->name = name;
	m_textureManager.registerTexture(texture);
	JobLoadTexture* job = new JobLoadTexture();
	job->nameOfTextureToLoad = name;
	job->texture = texture;
	job->textureData = asset::assetLoadTextureData(name);
	m_jobVector.push_back(job);
	return texture;
}

Material* Renderer::getMaterial(std::string name)
{
	Material* m = m_materialManager.getMaterialByName(name);
	if (m)
		return m;

	Material* material = new Material();
	JobLoadMaterial* job = new JobLoadMaterial();
	job->material = material;
	job->nameOfMaterialToLoad = name;
	job->materialData = asset::assetLoadMaterialData(name);
	m_jobVector.push_back(job);
	return material;
}

void Renderer::render()
{
	proccessLoadJobs();
	m_dx11Renderer.clearFrame();
	glm::mat4 pm = glm::perspective(glm::radians(40.0f), 16.0f / 9.0f, 0.1f, 1000.0f);

	for (std::pair<EntityUid, Entity*> pair : m_world->m_entityMap)
	{
		RenderComponent* rc = dynamic_cast<RenderComponent*>(pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT));
		if (!rc)
			continue;
		Model* m = rc->getModel();
		glm::mat4 mm = Transform::matrixFromTransform(pair.second->transform, true);
		m_dx11Renderer.renderModel(m, mm, m_camera.getViewMatrix() , m_camera.getPerspectiveMatrix(), NULL, m_standardShader);
	}
}

void Renderer::present()
{
	m_dx11Renderer.present();
}

void Renderer::proccessLoadJobs()
{
	if (m_jobVector.size() == 0)
		return;

	Job* job = m_jobVector.back();

	JobLoadModel* jobLoadModel = dynamic_cast<JobLoadModel*>(job);
	if (jobLoadModel)
	{
		if (!jobLoadModel->modelData->loaded)
			return;
		m_jobVector.pop_back();

		m_dx11Renderer.uploadModel(jobLoadModel);

		jobLoadModel->model->name = jobLoadModel->modelData->name;
		jobLoadModel->model->vertCount = jobLoadModel->modelData->vertCount;
		jobLoadModel->model->material = getMaterial(jobLoadModel->modelData->materialName);
		jobLoadModel->model->m_vertexSize = jobLoadModel->modelData->floatsPerVert * sizeof(float);
		jobLoadModel->model->m_dataSize = jobLoadModel->modelData->floatsPerVert * sizeof(float) * jobLoadModel->modelData->vertCount;
		m_modelManager.registerModel(jobLoadModel->model);

		delete jobLoadModel;
		return;
	}

	JobLoadTexture* jobLoadTexture = dynamic_cast<JobLoadTexture*>(job);
	if (jobLoadTexture)
	{
		if (!jobLoadTexture->textureData->loaded)
			return;
		m_jobVector.pop_back();

		m_dx11Renderer.uploadTexture(jobLoadTexture);

		jobLoadTexture->texture->name = jobLoadTexture->nameOfTextureToLoad;
		jobLoadTexture->texture->width = jobLoadTexture->textureData->width;
		jobLoadTexture->texture->height = jobLoadTexture->textureData->height;
		m_textureManager.registerTexture(jobLoadTexture->texture);

		delete jobLoadTexture;
		return;
	}

	JobLoadMaterial* jobLoadMaterial = dynamic_cast<JobLoadMaterial*>(job);
	if (jobLoadMaterial)
	{
		if (!jobLoadMaterial->materialData->loaded)
			return;
		m_jobVector.pop_back();

		jobLoadMaterial->material->name = jobLoadMaterial->nameOfMaterialToLoad;
		jobLoadMaterial->material->diffuseTexture = getTexture(jobLoadMaterial->materialData->diffuseTextureName.c_str());
		jobLoadMaterial->material->normalTexture = NULL;
		jobLoadMaterial->material->ormTexture = NULL;
		m_materialManager.registerMaterial(jobLoadMaterial->material);

		delete jobLoadMaterial;
		return;
	}

	LOGERROR("Unknown job sent to render!");
}

Entity* Renderer::colorPick(glm::vec2 cursorPosition)
{
	//std::unordered_map<glm::vec3, Entity*> colorEntityMap;
	glm::vec3 nextColor = {0.0f, 0.0f, 0.01f};

	struct ColorEntityPair
	{
		glm::vec3 color;
		Entity* entity;
	};

	std::vector<ColorEntityPair> colorEntities;

	m_dx11Renderer.clearColorPick(m_colorPickShader);

	for (std::pair<EntityUid, Entity*> pair : m_world->m_entityMap)
	{
		glm::vec3 color = nextColor;
		nextColor.z += 0.01f;
		if (nextColor.z == 1.01f)
		{
			nextColor.z = 0.0f;
			nextColor.y += 0.01f;
		}
		if (nextColor.y == 1.01f)
		{
			nextColor.y = 0.0f;
			nextColor.x += 0.01f;
		}

		ColorEntityPair cep = { color, pair.second };
		colorEntities.push_back(cep);

		RenderComponent* rc = dynamic_cast<RenderComponent*>(pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT));
		if (!rc)
			continue;
		Model* m = rc->getModel();
		glm::mat4 mm = Transform::matrixFromTransform(pair.second->transform, true);
		//m_dx11Renderer.renderModel(m, mm, m_camera.getViewMatrix(), m_camera.getPerspectiveMatrix(), NULL, m_standardShader);
		//DO COLORED RENDER
		m_dx11Renderer.renderColorPickModel(m, mm, m_camera.getViewMatrix(), m_camera.getPerspectiveMatrix(), m_colorPickShader, color);


	}

	glm::vec3 result = m_dx11Renderer.getColorPickColor(cursorPosition, m_colorPickShader);

	for (ColorEntityPair cep : colorEntities)
	{
		if (result == cep.color)
			return cep.entity;
	}

	return nullptr;
}

glm::vec3 Renderer::screenToWorldPosition(glm::vec2 cursorPosition)
{
	return m_dx11Renderer.screenToWorldPosition(cursorPosition, m_camera.getViewMatrix(), m_camera.getPerspectiveMatrix());
}