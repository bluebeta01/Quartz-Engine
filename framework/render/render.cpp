/*#include "framework/pch.h"
#include "render.h"
#include "c_shader.h"
#include "c_shadermanager.h"
#include "c_modelmanager.h"
#include "framework/asset/asset.h"
#include "framework/screen/screen.h"
#include "jobs.h"
#include "c_texturemanager.h"
#include "c_materialmanager.h"
#include "c_light.h"
#include "c_pointlight.h"
#include "framework/filesystem/filesystem.h"
#include "framework/common/entity/c_entity.h"
#include "framework/common/components/c_rendercomponent.h"
#include "framework/screen/screen.h"
#include "c_framebuffer.h"
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

#include <D3D11.h>
#include <D3DX11.h>

glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;

ModelManager modelManager;
TextureManager textureManager;
MaterialManager materialManager;
std::vector<Job*> jobVector;
std::vector<PointLight> pointLightsVector;
std::vector<Entity*>* entityVector;
FrameBuffer* currentFrameBuffer;

Renderer::Renderer(int viewportSizeX, int viewportSizeY)
{
	loadShader("standard");
	//loadShader("colorpick");
}

Renderer::~Renderer()
{
	
}

void Renderer::prepFrame()
{
	processRenderJobs();
	clearFrame();
}

Model* Renderer::getModel(std::string name)
{
	Model* m = modelManager.getModelByName(name);
	if (m)
		return m;

	Model* model = new Model();
	JobLoadModel* jobLoadModel = new JobLoadModel();
	jobLoadModel->nameOfModelToLoad = name;
	jobLoadModel->model = model;
	jobLoadModel->modelData = asset::assetLoadModelData(name);
	jobVector.push_back(jobLoadModel);
	return model;
}

Texture* Renderer::getTexture(std::string name)
{
	Texture* t = textureManager.getTextureByName(name);
	if (t)
		return t;

	Texture* texture = new Texture();
	texture->name = name;
	textureManager.registerTexture(texture);
	JobLoadTexture* job = new JobLoadTexture();
	job->nameOfTextureToLoad = name;
	job->texture = texture;
	job->textureData = asset::assetLoadTextureData(name);
	jobVector.push_back(job);
	return texture;
}

Material* Renderer::getMaterial(std::string name)
{
	Material* m = materialManager.getMaterialByName(name);
	if (m)
		return m;

	Material* material = new Material();
	JobLoadMaterial* job = new JobLoadMaterial();
	job->material = material;
	job->nameOfMaterialToLoad = name;
	job->materialData = asset::assetLoadMaterialData(name);
	jobVector.push_back(job);
	return material;
}

void Renderer::setProjectionMatrix(glm::mat4 matrix)
{
	projectionMatrix = matrix;
}

void Renderer::setViewMatrix(glm::mat4 matrix)
{
	viewMatrix = matrix;
}

void Renderer::setViewportSize(int x, int y)
{
	viewportSizeX = x;
	viewportSizeY = y;
}

void Renderer::drawEntities(std::vector<Entity*> entities)
{
	
}

void Renderer::setLightsVector(std::vector<PointLight> pointLights)
{
	pointLightsVector = pointLights;
}

void Renderer::setFrameBuffer(FrameBuffer* frameBuffer)
{
	currentFrameBuffer = frameBuffer;
	
}

void Renderer::parseModel(JobLoadModel* jobLoadModel)
{
	if (!jobLoadModel->modelData->loaded)
		return;
	
	jobVector.pop_back();
	
	//unsigned int vao;
	//unsigned int vertsVbo;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glGenBuffers(1, &vertsVbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vertsVbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * jobLoadModel->modelData->vertCount * jobLoadModel->modelData->floatsPerVert, jobLoadModel->modelData->data, GL_STATIC_DRAW);

	////Stride is the size of a whole vertex
	//glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	//glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(float) * jobLoadModel->modelData->vertCount * jobLoadModel->modelData->floatsPerVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screen::dxDevice->CreateBuffer(&bd, NULL, &jobLoadModel->model->dxVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	screen::dxDeviceContext->Map(jobLoadModel->model->dxVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, jobLoadModel->modelData->data, sizeof(float) * jobLoadModel->modelData->vertCount * jobLoadModel->modelData->floatsPerVert);
	screen::dxDeviceContext->Unmap(jobLoadModel->model->dxVertexBuffer, NULL);


	jobLoadModel->model->name = jobLoadModel->modelData->name;
	jobLoadModel->model->vertCount = jobLoadModel->modelData->vertCount;
	jobLoadModel->model->material = getMaterial(jobLoadModel->modelData->materialName);
	modelManager.registerModel(jobLoadModel->model);

	delete jobLoadModel;
}

void Renderer::processRenderJobs()
{
	if (jobVector.size() == 0)
		return;

	Job* job = jobVector.back();

	JobLoadModel* jobLoadModel = dynamic_cast<JobLoadModel*>(job);
	if (jobLoadModel)
	{
		parseModel(jobLoadModel);
		return;
	}

	JobLoadTexture* jobLoadTexture = dynamic_cast<JobLoadTexture*>(job);
	if (jobLoadTexture)
	{
		if (!jobLoadTexture->textureData->loaded)
			return;

		jobVector.pop_back();

		jobLoadTexture->texture->name = jobLoadTexture->nameOfTextureToLoad;
		jobLoadTexture->texture->width = jobLoadTexture->textureData->width;
		jobLoadTexture->texture->height = jobLoadTexture->textureData->height;
		//glGenTextures(1, &jobLoadTexture->texture->glid);
		//glBindTexture(GL_TEXTURE_2D, jobLoadTexture->texture->glid);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jobLoadTexture->textureData->width, jobLoadTexture->textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, jobLoadTexture->textureData->rawPixelData.data());
		//glGenerateMipmap(GL_TEXTURE_2D);

		//textureManager.registerTexture(jobLoadTexture->texture);

		delete jobLoadTexture;
		return;
	}

	JobLoadMaterial* jobLoadMaterial = dynamic_cast<JobLoadMaterial*>(job);
	if (jobLoadMaterial)
	{
		if (!jobLoadMaterial->materialData->loaded)
			return;

		jobVector.pop_back();

		jobLoadMaterial->material->name = jobLoadMaterial->nameOfMaterialToLoad;
		jobLoadMaterial->material->diffuseTexture = getTexture(jobLoadMaterial->materialData->diffuseTextureName.c_str());
		jobLoadMaterial->material->normalTexture = NULL;
		jobLoadMaterial->material->ormTexture = NULL;
		materialManager.registerMaterial(jobLoadMaterial->material);

		delete jobLoadMaterial;
		return;
	}

	LOGERROR("Unknown job sent to render!");
}

void Renderer::loadShader(std::string name)
{
	std::string vpath;
	vpath.append("shaders/dx/");
	vpath.append(name);
	vpath.append(".hlsl");

	vpath = filesystem::findFilePathByName(vpath);

	std::string fpath;
	fpath.append("shaders/dx/");
	fpath.append(name);
	fpath.append(".hlsl");

	fpath = filesystem::findFilePathByName(fpath);

	Shader* shader = new Shader(name, vpath, fpath);
	shaderManager.registerShader(shader);
}

glm::mat4 Renderer::matrixFromTransform(Transform transform, bool correctRotation)
{
	glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), transform.position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

	glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x - 90.0f), glm::vec3(1, 0, 0));
	glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	glm::mat4 rotationMatrix = rotationMatrixZ * rotationMatrixY * rotationMatrixX;

	return positionMatrix * rotationMatrix * scaleMatrix;
}

void Renderer::renderModel(Model* model, Shader* shader, unsigned int shaderConstantBufferCount, SHADER_CONSTANT_BUFFER* shaderConstantBufferArray)
{
	screen::dxDeviceContext->VSSetShader(shader->dxVertexShader, 0, 0);
	screen::dxDeviceContext->PSSetShader(shader->dxPixelShader, 0, 0);
	screen::dxDeviceContext->IASetInputLayout(shader->dxBufferLayout);


	UINT stride = sizeof(float) * 8;
	UINT offset = 0;
	screen::dxDeviceContext->IASetVertexBuffers(0, 1, &model->dxVertexBuffer, &stride, &offset);


	for (int i = 0; i < shaderConstantBufferCount; i++)
	{
		ID3D11Buffer* constantBuffer;
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = shaderConstantBufferArray[i].bufferDataSize + (16 - shaderConstantBufferArray[i].bufferDataSize);
		desc.StructureByteStride = 0;
		screen::dxDevice->CreateBuffer(&desc, 0, &constantBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		screen::dxDeviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, shaderConstantBufferArray[i].data, shaderConstantBufferArray[i].bufferDataSize);
		screen::dxDeviceContext->Unmap(constantBuffer, 0);

		screen::dxDeviceContext->VSSetConstantBuffers(shaderConstantBufferArray[i].slotValue, 1, &constantBuffer);

		constantBuffer->Release();
	}


	screen::dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	screen::dxDeviceContext->Draw(model->vertCount, 0);
}

void Renderer::clearFrame()
{
	screen::dxDeviceContext->ClearRenderTargetView(screen::dxBackBuffer, glm::value_ptr(glm::vec4(0, 0.2, 0.4, 1)));
}*/