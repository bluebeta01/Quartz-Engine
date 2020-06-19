#pragma once
#include "c_job.h"
#include "c_model.h"
#include "asset/c_modeldata.h"
#include "c_texture.h"
#include "asset/c_texturedata.h"
#include "c_material.h"
#include "asset/c_materialdata.h"

class JobLoadModel : public Job
{
public:
	std::string nameOfModelToLoad;
	Model * model;
	ModelData* modelData;

	~JobLoadModel()
	{
		delete modelData;
	}
};

class JobLoadTexture : public Job
{
public:
	std::string nameOfTextureToLoad;
	Texture * texture;
	TextureData * textureData;

	~JobLoadTexture()
	{
		delete textureData;
	}
};

class JobLoadMaterial : public Job
{
public:
	std::string nameOfMaterialToLoad;
	Material * material;
	MaterialData * materialData;

	~JobLoadMaterial()
	{
		delete materialData;
	}
};