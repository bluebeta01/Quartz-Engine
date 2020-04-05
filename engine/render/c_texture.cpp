#include "pch.h"
#include "c_texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{
	m_dxTexture->Release();
	m_dxResourceView->Release();
}