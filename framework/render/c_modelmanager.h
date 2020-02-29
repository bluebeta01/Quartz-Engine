#pragma once
#include "c_model.h"
#include "pch.h"

static class ModelManager
{
public:
	static Model* getModelByName(std::string name);
	static void registerModel(Model* model);

private:
	static std::vector<Model*> models;
};