#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "resource_dir.h"
#include "mainloop.h"

int main(void)
{	
	// set resources directory for easy access
  SearchAndSetResourceDir("resources");
  
	// initialize window
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1920, 1080, "Stuff From Space!");
	SetExitKey(0);

	// initialize audio device
	InitAudioDevice();
	SetMasterVolume(0.5f);
  
	// run main menu
  MainLoop();

	// detach audio device
	CloseAudioDevice();
  
	// destroy the window and cleanup the OpenGL context
	CloseWindow();

	return(0);
}
