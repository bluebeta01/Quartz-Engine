#pragma once
#include "framework/pch.h"
#include "c_material.h"

class Model
{
public:
	ID3D11Buffer* dxVertexBuffer;
	unsigned int vertCount;
	std::string name;
	Material* material;
};