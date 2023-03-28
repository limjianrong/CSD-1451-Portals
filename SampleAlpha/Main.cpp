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
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "GameState_Platformer.hpp"

// ---------------------------------------------------------------------------
// main
s8 Albam_fontID;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{	

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2); //ensures that the application window does not 
																			   //exceed the size of the user's monitor resolution
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	//_crtBreakAlloc = 555; // (type in the memory allocation number)

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, WINDOWLENGTH_X, WINDOWLENGTH_Y, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("CSD 1451 Portals");

	// Set background color
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 100.0f);

	// Loads fontID into memory
	Albam_fontID = AEGfxCreateFont("Assets/Albam.ttf", 50);
	
	// Initialize GameStateManager to initialize the starting state
	GameStateMgrInit(GS_SplashScreen);

	// Loop while the program should not quit
	while (gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, update and load the gamestate
		if (gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
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

	// Destroy fontID from memory
	AEGfxDestroyFont(Albam_fontID);
	// free the system
	AESysExit();
}
