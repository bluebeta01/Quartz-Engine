#pragma once
#include "framework/common/c_job.h"
#include "c_model.h"
#include "framework/asset/c_modeldata.h"
#include "c_texture.h"
#include "framework/asset/c_texturedata.h"
#include "c_material.h"
#include "framework/asset/c_materialdata.h"

class JobLoadModel : public Job
{
public:
	Model * model;
	std::string nameOfModelToLoad;
	ModelData* modelData;

	~JobLoadModel()
	{
		delete modelData;
	}
};

class JobLoadTexture : public Job
{
public:
	Texture * texture;
	std::string nameOfTextureToLoad;
	TextureData * textureData;

	~JobLoadTexture()
	{
		delete textureData;
	}
};

class JobLoadMaterial : public Job
{
public:
	Material * material;
	std::string nameOfMaterialToLoad;
	MaterialData * materialData;

	~JobLoadMaterial()
	{
		delete materialData;
	}
};