#pragma once
#include "c_texture.h"
#include "pch.h"

class TextureManager
{
public:
	std::vector<Texture*> textures;

	void registerTexture(Texture* texture)
	{
		textures.push_back(texture);
	}

	Texture* getTextureByName(std::string name)
	{
		for (int i = 0; i < textures.size(); i++)
		{
			if (textures[i]->name == name)
				return textures[i];
		}
		return NULL;
	}
};