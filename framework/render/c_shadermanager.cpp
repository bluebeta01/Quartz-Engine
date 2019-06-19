#include "framework/pch.h"
#include "c_shadermanager.h"

std::vector<Shader*> ShaderManager::shaders;

void ShaderManager::registerShader(Shader* shader)
{
	shaders.push_back(shader);
}

Shader* ShaderManager::getShaderByName(std::string name)
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (shaders[i]->name == name)
			return shaders[i];
	}
	return NULL;
}