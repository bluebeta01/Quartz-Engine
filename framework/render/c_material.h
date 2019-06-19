#pragma once
#include "framework/pch.h"
#include "c_texture.h"

class Material
{
public:
	std::string name;

	Texture * diffuseTexture;
	Texture * ormTexture;
	Texture * normalTexture;
};