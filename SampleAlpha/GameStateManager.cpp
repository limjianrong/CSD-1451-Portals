/*==================================================================================
* All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameStateManager.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This source file contains functions that initialize the gameState and updates the gameState

  The function includes:
  - GameStateMgrInit
	Initialise the gameState variables

  - GameStateMgrUpdate
	Updates the gameState based on current input
==================================================================================*/

#include "GameStateManager.hpp"
#include "GameState_Platformer.hpp"
#include "GameState_Mainmenu.hpp"
#include "Settings.hpp"

#include "GameState_Win.hpp"
#include "GameState_Lose.hpp"
#include "GameState_Splashscreen.hpp"
#include "GameState_Credits.hpp"

u32 gGameStateInit, gGameStateCurr, gGameStatePrev, gGameStateNext;

FP GameStateLoad = nullptr, GameStateInit = nullptr, GameStateUpdate = nullptr, GameStateDraw = nullptr, GameStateFree = nullptr, GameStateUnload = nullptr;

/******************************************************************************/
/*!
\brief
Set the starting state to the given game state parameter and sets all current, previous and next game states to this starting state before calling the update function

\param unsigned int gameStateInit
An enum to the first state to set the program to

\return void
*/
/******************************************************************************/
void GameStateMgrInit(u32 gameStateInit)
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previoud and next game
	gGameStateCurr = gGameStatePrev = gGameStateNext = gGameStateInit;

	// call the update to set the function pointers
	GameStateMgrUpdate();
}

/******************************************************************************/
/*!
\brief
Update the game state's function pointers based on what the current state should be

\param void

\return void
*/
/******************************************************************************/
void GameStateMgrUpdate()
{
	// Check if the program is qutting or restarting
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;


	// Switch based on what the current state should be
	switch (gGameStateCurr)
	{
		case GS_MainMenu: // Main Menu
			GameStateLoad	= GameStateMainmenuLoad;
			GameStateInit	= GameStateMainmenuInit;
			GameStateUpdate = GameStateMainmenuUpdate;
			GameStateDraw	= GameStateMainmenuDraw;
			GameStateFree	= GameStateMainmenuFree;
			GameStateUnload = GameStateMainmenuUnload;
			break;
		case GS_Platformer:	// Game itself
			// Assign the respective function pointers
			GameStateLoad	= GameStatePlatformerLoad;
			GameStateInit	= GameStatePlatformerInit;
			GameStateUpdate = GameStatePlatformerUpdate;
			GameStateDraw	= GameStatePlatformerDraw;
			GameStateFree	= GameStatePlatformerFree;
			GameStateUnload = GameStatePlatformerUnload;
			break;
		case GS_Win:
			GameStateLoad	= GameStateWinLoad;
			GameStateInit	= GameStateWinInit;
			GameStateUpdate = GameStateWinUpdate;
			GameStateDraw	= GameStateWinDraw;
			GameStateFree	= GameStateWinFree;
			GameStateUnload = GameStateWinUnload;
			break;
		case GS_Lose:
			GameStateLoad	= GameStateLoseLoad;
			GameStateInit	= GameStateLoseInit;
			GameStateUpdate = GameStateLoseUpdate;
			GameStateDraw	= GameStateLoseDraw;
			GameStateFree	= GameStateLoseFree;
			GameStateUnload = GameStateLoseUnload;
			break;
		case GS_SplashScreen:
			GameStateLoad	= GameStateSplashscreenLoad;
			GameStateInit	= GameStateSplashscreenInit;
			GameStateUpdate = GameStateSplashscreenUpdate;
			GameStateDraw	= GameStateSplashscreenDraw;
			GameStateFree	= GameStateSplashscreenFree;
			GameStateUnload = GameStateSplashscreenUnload;
			break;
		case GS_Credits:
			GameStateLoad	= GameStateCreditsLoad;
			GameStateInit	= GameStateCreditsInit;
			GameStateUpdate = GameStateCreditsUpdate;
			GameStateDraw	= GameStateCreditsDraw;
			GameStateFree	= GameStateCreditsFree;
			GameStateUnload = GameStateCreditsUnload;
			break;
		default:	// Error handling
			AE_FATAL_ERROR("invalid state!!");
	}
}