#pragma once
#include "pch.h"
#include "c_modeldata.h"
#include "c_texturedata.h"
#include "c_materialdata.h"

namespace asset
{
	std::thread assetInit();
	std::string loadText(std::string path);
	ModelData* assetLoadModelData(std::string filename);
	TextureData* assetLoadTextureData(std::string filename);
	MaterialData* assetLoadMaterialData(std::string filename);
}