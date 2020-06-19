#include "c_editor.h"
#include "engine/engine.h"

int main()
{
	Editor editor;

	while (!GameWindow::s_isTerminating)
	{
		editor.tick();
		editor.render();
	}

	editor.terminate();
}