#pragma once
#include "c_material.h"
#include "pch.h"

class MaterialManager
{
public:
	std::vector<Material*> materials;

	void registerMaterial(Material* material)
	{
		materials.push_back(material);
	}

	Material* getMaterialByName(std::string name)
	{
		for (int i = 0; i < materials.size(); i++)
		{
			if (materials[i]->name == name)
				return materials[i];
		}
		return NULL;
	}
};