#pragma once
#include "framework/pch.h"
#include "framework/common/c_job.h"
#include "c_modeldata.h"
#include "c_texturedata.h"
#include "c_materialdata.h"

class JobLoadModelData : public Job
{
public:
	ModelData * modelData;
	std::string filename;
};

class JobLoadTextureData : public Job
{
public:
	TextureData * textureData;
	std::string filename;
};

class JobLoadMaterialData : public Job
{
public:
	MaterialData * materialData;
	std::string filename;
};