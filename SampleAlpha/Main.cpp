/*!**************************************************************************************************
\file     Main.cpp
\author   ZhaoZhi, ZhiYee, JianRong, ZhunHan
\par      
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  Main function of the game

  The function includes:
  - create_Square_Mesh
	Creates a square mesh of width 1 and height 1
*****************************************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Player.hpp"
#include "Utilities.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "draw_level.hpp"
#include "Enemy.hpp"
#include "GameState_Mainmenu.hpp"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "GameState_Platformer.hpp"

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

	// Changing the window title
	AESysSetWindowTitle("CSD 1451 Portals");
	// reset the system modules
	AESysReset();

	//set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Initialize GameStateManager to initialize the starting state
	GameStateMgrInit(GS_Mainmenu);

	// Loop while the program should not quit
	while (gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, update and load the gamestate
		if (gGameStateCurr != GS_RESTART)
		{
			GameStateLoad();
			GameStateMgrUpdate();
			
		}
		else // Otherwise
			gGameStateNext = gGameStateCurr = gGameStatePrev;	// Set next and current state to the previous state

		// Initialize the gamestate
		GameStateInit();

		// Loop while we should stay in current state
		while (gGameStateCurr == gGameStateNext)
		{
			AESysFrameStart();	// Call start of frame rate controller
			AEInputUpdate();	// Call event handler
			GameStateUpdate();	// Update current state's object instances
			GameStateDraw();	// Draw current state's object instances
			AESysFrameEnd();	// Call end of frame rate controller

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				gGameStateNext = GS_QUIT;
		}

		// Current state should end: Free the state
		GameStateFree();

		// Check if the next state is to not restart the current state
		if (gGameStateNext != GS_RESTART)
			GameStateUnload();	// Case is true: Unload the current state

		gGameStatePrev = gGameStateCurr;	// Set previous state to current state
		gGameStateCurr = gGameStateNext;	// Set current state to next state
	}

	// free the system
	AESysExit();
}


/*initialize_player(200);
initialize_portal();
initialize_endpoint();
draw_level_init();
enemy_init();*/

//	// Game Loop
//	while (gGameRunning)
//	{
//		// Informing the system about the loop's start
//		AESysFrameStart();
//
//		// Handling Input
//		AEInputUpdate();
//
//		// Set the background to blue
//		AEGfxSetBackgroundColor(0.0f, 75.3f, 79.6f);
//		// Tell the engine to get ready to draw something with texture.
//		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//		// Set the tint to white, so that the sprite can display the full range of colors (default is black).
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
//		// Set blend mode to AE_GFX_BM_BLEND
//		// This will allow transparency.
//		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//		AEGfxSetTransparency(1.0f);
//
//
//		draw_player(200);
//
//		draw_enemy();
//		
//		draw_level();
//		
//		// Informing the system about the loop's end
//		AESysFrameEnd();
//
//		// check if forcing the application to quit
//		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
//			gGameRunning = 0;
//	}
//
//	//AEGfxMeshFree(pMesh);
//	//AEGfxTextureUnload(pTex);
//	
//	// free the system
//	AESysExit();
//}
