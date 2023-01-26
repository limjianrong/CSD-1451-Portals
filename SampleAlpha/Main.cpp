// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Player.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "draw_level.hpp"

#define WINDOWXLENGTH 1000
#define WINDOWYLENGTH 600
// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{	

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2); //ensures that the application window does not 
																			   //exceed the size of the user's monitor resolution
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, WINDOWXLENGTH, WINDOWYLENGTH, 1, 60, true, NULL);
	

	initialize_player(200);
	initialize_portal();
	bullet_initialise();
	draw_level_init();

	// Changing the window title
	AESysSetWindowTitle("CSD 1451 Portals");
	
	
	// reset the system modules
	AESysReset();

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Set the background to blue
		AEGfxSetBackgroundColor(0.0f, 75.3f, 79.6f);
		// Tell the engine to get ready to draw something with texture.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can display the full range of colors (default is black).
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND
		// This will allow transparency.
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);


		draw_player(200);
		
		draw_level();
		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	//AEGfxMeshFree(pMesh);
	//AEGfxTextureUnload(pTex);
	
	// free the system
	AESysExit();
}
