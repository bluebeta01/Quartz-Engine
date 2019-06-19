#pragma once
#include "framework/pch.h"
#include "c_dataobject.h"

class MaterialData : public DataObject
{
public:
	std::string diffuseTextureName;
	std::string ormTextureName;
	std::string normalTextureName;
};