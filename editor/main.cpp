#include "c_editor.h"

int main()
{
	Editor editor;

	while (!screen::terminated)
	{
		editor.tick();
		editor.render();
	}

	editor.terminate();
}