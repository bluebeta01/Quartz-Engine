#include "framework/pch.h"
#include "c_modelmanager.h"

std::vector<Model*> ModelManager::models;

Model* ModelManager::getModelByName(std::string name)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (models[i]->name == name)
			return models[i];
	}
	return NULL;
}

void ModelManager::registerModel(Model* model)
{
	models.push_back(model);
}
