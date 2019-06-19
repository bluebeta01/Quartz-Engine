#pragma once
#include "framework/pch.h"
#include "c_dataobject.h"

class ModelData : public DataObject
{
public:
	float* data;
	int vertCount;
	int floatsPerVert;
	std::string name;
	std::string materialName;

	~ModelData()
	{
		delete data;
	}
};