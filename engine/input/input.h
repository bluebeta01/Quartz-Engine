#pragma once
#include "pch.h"

namespace input
{
	enum MOUSEBUTTON
	{
		MOUSEBUTTON_LEFT, MOUSEBUTTON_RIGHT, MOUSEBUTTON_MIDDLE
	};

	extern glm::vec2 cursorPosition;
	extern glm::vec2 cursorDelta;
	extern bool cursorLock;
	extern glm::vec2 cursorLockPosition;

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