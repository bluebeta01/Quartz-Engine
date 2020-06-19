#pragma once
#include "pch.h"
#include "c_material.h"

class Model
{
public:
	std::string name;
	ID3D11Buffer* dxVertexBuffer;
	UINT vertCount;
	UINT m_vertexSize;
	UINT m_dataSize;
	Material* material;
};