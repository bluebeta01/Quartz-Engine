#pragma once
#include "framework/pch.h"

namespace input
{
	enum MOUSEBUTTON
	{
		MOUSEBUTTON_LEFT, MOUSEBUTTON_RIGHT, MOUSEBUTTON_MIDDLE
	};

	extern glm::vec2 mousePosition;
	extern glm::vec2 mouseDelta;
	extern bool lockMouse;
	extern glm::vec2 mouseLockPosition;

	void registerKeyDown(int key);
	void registerKeyUp(int key);
	void registerKeyPressed(int key);
	void clearKeysPressed();
	bool isKeyDown(int key);
	bool isKeyPressed(int key);

	void registerMouseButtonDown(MOUSEBUTTON button);
	void registerMouseButtonUp(MOUSEBUTTON button);
	void registerMouseButtonClick(MOUSEBUTTON button);
	void clearMouseButtonsClicked();
	bool isMouseButtonDown(MOUSEBUTTON button);
	bool isMouseButtonClicked(MOUSEBUTTON button);
}