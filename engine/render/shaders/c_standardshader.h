#pragma once
#include "render/c_shader.h"
#include <D3D11.h>

class StandardShader : public Shader
{
public:
	ID3D11Buffer* m_matrixBuffer = nullptr;

	StandardShader(ID3D11Device* device);
};