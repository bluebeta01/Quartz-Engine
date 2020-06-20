#pragma once
#include <engine/engine.h>
#include "c_window.h"

class RenderWindow : public Window
{
public:
	int m_width;
	int m_height;
	bool m_resized;
	Framebuffer* m_framebuffer;

	RenderWindow();
	void render(ID3D11Device* device);

private:
	ID3D11ShaderResourceView* m_framebufferView;
};