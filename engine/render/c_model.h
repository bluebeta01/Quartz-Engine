#pragma once
#include "pch.h"
#include "c_material.h"

class Model
{
public:
	ID3D11Buffer* dxVertexBuffer;
	UINT vertCount;
	UINT m_vertexSize;
	UINT m_dataSize;
	std::string name;
	Material* material;
};