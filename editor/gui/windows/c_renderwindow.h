#pragma once
#include "c_guiwindow.h"
#include <framework/framework.h>

class RenderWindow : public GUIWindow
{
public:
	FrameBuffer* sceneFrameBuffer;
	Camera* camera;
	bool amLooking = false;
	bool leftClicked = false;
	glm::vec2 windowSize;
	glm::vec2 localCursorPosition;
	RenderWindow();
	~RenderWindow();
	void render();

private:
	float lastWindowSizeX, lastWindowSizeY;
};