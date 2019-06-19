#pragma once
#include "c_shader.h"
#include "framework/pch.h"

static class ShaderManager
{
public:
	static void registerShader(Shader* shader);
	static Shader* getShaderByName(std::string name);
private:
	static std::vector<Shader*> shaders;
};