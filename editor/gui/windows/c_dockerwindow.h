#pragma once
#include "c_guiwindow.h"

class DockerWindow : public GUIWindow
{
public:
	DockerWindow();
	~DockerWindow();
	void render();
};