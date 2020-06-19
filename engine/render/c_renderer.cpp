#include "pch.h"
#include "c_renderer.h"
#include "jobs.h"
#include "asset/asset.h"
#include "filesystem/filesystem.h"
#include "c_gamewindow.h"

Renderer::Renderer(World* world) : m_world(world)
{

}

void Renderer::initialize(GameWindow* gameWindow)
{
	m_width = gameWindow->s_clientSize.x;
	m_height = gameWindow->s_clientSize.y;
	m_currentCamera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 40.0f, 0.1f, 1000.0f, (float)m_width / (float)m_height);
	m_dx11Renderer.initialize(gameWindow);
	m_standardShader = new StandardShader(m_dx11Renderer.m_device);
	m_colorPickShader = new ColorPickShader(m_dx11Renderer.m_device);
}

Framebuffer* Renderer::createFramebuffer()
{
	Framebuffer* frameBuffer = new Framebuffer();
	frameBuffer->initialize(m_dx11Renderer.m_device, m_width, m_height);
	return frameBuffer;
}

Model* Renderer::getModel(std::string name)
{
	Model* m = m_modelManager.getModelByName(name);
	if (m)
		return m;

	Model* model = new Model();
	model->name = name;
	m_modelManager.registerModel(model);
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

void Renderer::renderModel(Model* model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
	Material* overrideMaterial, Framebuffer* frameBuffer, bool depth)
{
	m_dx11Renderer.enableDepth(depth);
	m_dx11Renderer.renderModel(model, modelMatrix, viewMatrix, projectionMatrix, overrideMaterial, m_standardShader, frameBuffer);
	m_dx11Renderer.enableDepth(depth);
}

void Renderer::clearFramebuffer(Framebuffer* framebuffer)
{
	m_dx11Renderer.clearFramebuffer(framebuffer);
}

void Renderer::onResize()
{
	m_width = GameWindow::s_clientSize.x;
	m_height = GameWindow::s_clientSize.y;
	m_dx11Renderer.onResize();
	m_colorPickShader->screenResize(GameWindow::s_clientSize.x, GameWindow::s_clientSize.y);
}

void Renderer::tick()
{
	if (GameWindow::s_resizeEvent)
		onResize();
}

void Renderer::render()
{
	proccessLoadJobs();
	m_dx11Renderer.clearFrame();
	m_dx11Renderer.enableDepth(true);
	glm::mat4 pm = glm::perspective(glm::radians(40.0f), (float)m_width / (float)m_height, 0.1f, 1000.0f);

	std::vector<Entity*> onTopModels;

	for (std::pair<EcsUid, Entity*> pair : m_world->m_entityMap)
	{
		if (!pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))
			continue;

		if (pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT)->enabled)
		{
			RenderComponent* rc = dynamic_cast<RenderComponent*>(pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT));
			if (!rc)
				continue;
			Model* m = rc->getModel();
			if (rc->m_onTop)
			{
				onTopModels.push_back(pair.second);
				continue;
			}
			glm::mat4 mm = pair.second->getModelMatrix();
			m_dx11Renderer.renderModel(m, mm, m_currentCamera.getViewMatrix(), m_currentCamera.getPerspectiveMatrix(), NULL, m_standardShader, nullptr);
		}
	}

	m_dx11Renderer.enableDepth(false);
	for (Entity* e : onTopModels)
	{
		RenderComponent* rc = dynamic_cast<RenderComponent*>(e->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT));
		Model* m = rc->getModel();
		glm::mat4 mm = e->getModelMatrix();
		m_dx11Renderer.renderModel(m, mm, m_currentCamera.getViewMatrix(), m_currentCamera.getPerspectiveMatrix(), NULL, m_standardShader, nullptr);
	}
	m_dx11Renderer.enableDepth(true);
	
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

	std::vector<Entity*> onTopModels;

	for (std::pair<EcsUid, Entity*> pair : m_world->m_entityMap)
	{
		if (!pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT))
			continue;

		RenderComponent* rc = dynamic_cast<RenderComponent*>(pair.second->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT));
		if (!rc)
			continue;
		if (rc->m_onTop)
		{
			onTopModels.push_back(pair.second);
			continue;
		}

		glm::vec3 color = nextColor;
		nextColor.z += 0.02f;
		if (nextColor.z > 1.00f)
		{
			nextColor.z = 0.0f;
			nextColor.y += 0.02f;
		}
		if (nextColor.y > 1.00f)
		{
			nextColor.y = 0.0f;
			nextColor.x += 0.02f;
		}

		ColorEntityPair cep = { color, pair.second };
		colorEntities.push_back(cep);

		Model* m = rc->getModel();
		glm::mat4 mm = pair.second->getModelMatrix();
		//m_dx11Renderer.renderModel(m, mm, m_camera.getViewMatrix(), m_camera.getPerspectiveMatrix(), NULL, m_standardShader);
		//DO COLORED RENDER
		m_dx11Renderer.renderColorPickModel(m, mm, m_currentCamera.getViewMatrix(), m_currentCamera.getPerspectiveMatrix(), m_colorPickShader, color);
	}


	m_dx11Renderer.enableDepth(false);
	for(Entity* e : onTopModels)
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

		ColorEntityPair cep = { color, e };
		colorEntities.push_back(cep);

		RenderComponent* rc = dynamic_cast<RenderComponent*>(e->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT));
		Model* m = rc->getModel();
		glm::mat4 mm = e->getModelMatrix();
		m_dx11Renderer.renderColorPickModel(m, mm, m_currentCamera.getViewMatrix(), m_currentCamera.getPerspectiveMatrix(), m_colorPickShader, color);
	}
	m_dx11Renderer.enableDepth(true);

	glm::vec3 result = m_dx11Renderer.getColorPickColor(cursorPosition, m_colorPickShader);

	for (ColorEntityPair cep : colorEntities)
	{
		const double ep = 1e-5;
		bool equalX = std::abs(result.x - cep.color.x) <= ep * std::abs(result.x);
		bool equalZ = std::abs(result.z - cep.color.z) <= ep * std::abs(result.z);
		bool equalY = std::abs(result.y - cep.color.y) <= ep * std::abs(result.y);
		if (equalX && equalY && equalZ)
			return cep.entity;
	}
	
	return nullptr;
}

glm::vec3 Renderer::screenToWorldPosition(glm::vec2 cursorPosition, Framebuffer* framebuffer)
{
	return m_dx11Renderer.screenToWorldPosition(cursorPosition, m_currentCamera.getViewMatrix(), m_currentCamera.getPerspectiveMatrix(), framebuffer);
}