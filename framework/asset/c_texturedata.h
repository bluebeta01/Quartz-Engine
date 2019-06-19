#pragma once
#include "framework/pch.h"
#include "c_dataobject.h"

class TextureData : public DataObject
{
public:
	std::vector<unsigned char> rawPixelData;
	unsigned int width;
	unsigned int height;

	~TextureData()
	{
		
	}
};