#pragma once
#include "c_model.h"
#include "pch.h"

static class ModelManager
{
public:
	static void registerModel(Model* model);
	static Model* getModelByName(std::string name);

private:
	static std::vector<Model*> models;
};