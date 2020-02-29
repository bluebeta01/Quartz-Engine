#pragma once
#include "pch.h"
#include "c_modeldata.h"
#include "c_texturedata.h"
#include "c_materialdata.h"

namespace asset
{
	std::thread assetInit();

	ModelData* assetLoadModelData(std::string filename);
	TextureData* assetLoadTextureData(std::string filename);
	MaterialData* assetLoadMaterialData(std::string filename);

	std::string loadText(std::string path);
}