#pragma once
#include "pch.h"

class Texture
{
public:
	Texture();
	~Texture();
	std::string name;
	int width;
	int height;
	ID3D11ShaderResourceView* m_dxResourceView = nullptr;
	ID3D11Texture2D* m_dxTexture = nullptr;
};