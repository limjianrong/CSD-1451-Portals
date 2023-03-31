/*!**************************************************************************************************
\file     GameStateManager.cpp
\author   Lin ZhaoZhi
\par      DP email: z.lin@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file contains functions that initialize the gameState and updates the gameState

  The function includes:
  - GameStateMgrInit
	Initialise the gameState variables

  - GameStateMgrUpdate
	Updates the gameState based on current input
*****************************************************************************************************/

#include "GameStateManager.hpp"
#include "GameState_Platformer.hpp"
#include "GameState_Mainmenu.hpp"
#include "Settings.hpp"

#include "GameState_Win.hpp"
#include "GameState_Lose.hpp"
#include "GameState_Tutorial.hpp"
#include "GameState_Tutorial2.hpp"
#include "GameState_Splashscreen.hpp"

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
			GameStateLoad = GameStateMainmenuLoad;
			GameStateInit = GameStateMainmenuInit;
			GameStateUpdate = GameStateMainmenuUpdate;
			GameStateDraw = GameStateMainmenuDraw;
			GameStateFree = GameStateMainmenuFree;
			GameStateUnload = GameStateMainmenuUnload;
			break;
		case GS_Platformer:	// Game itself
			// Assign the respective function pointers
			GameStateLoad = GameStatePlatformerLoad;
			GameStateInit = GameStatePlatformerInit;
			GameStateUpdate = GameStatePlatformerUpdate;
			GameStateDraw = GameStatePlatformerDraw;
			GameStateFree = GameStatePlatformerFree;
			GameStateUnload = GameStatePlatformerUnload;
			break;
		case GS_Win:
			GameStateLoad = GameStateWinLoad;
			GameStateInit = GameStateWinInit;
			GameStateUpdate = GameStateWinUpdate;
			GameStateDraw = GameStateWinDraw;
			GameStateFree = GameStateWinFree;
			GameStateUnload = GameStateWinUnload;
			break;
		case GS_Lose:
			GameStateLoad = GameStateLoseLoad;
			GameStateInit = GameStateLoseInit;
			GameStateUpdate = GameStateLoseUpdate;
			GameStateDraw = GameStateLoseDraw;
			GameStateFree = GameStateLoseFree;
			GameStateUnload = GameStateLoseUnload;
			break;
		case GS_SplashScreen:
			GameStateLoad = GameStateSplashscreenLoad;
			GameStateInit = GameStateSplashscreenInit;
			GameStateUpdate = GameStateSplashscreenUpdate;
			GameStateDraw = GameStateSplashscreenDraw;
			GameStateFree = GameStateSplashscreenFree;
			GameStateUnload = GameStateSplashscreenUnload;
			break;
		case GS_Tutorial:
			GameStateLoad = GameStateTutorialLoad;
			GameStateInit = GameStateTutorialInit;
			GameStateUpdate = GameStateTutorialUpdate;
			GameStateDraw = GameStateTutorialDraw;
			GameStateFree = GameStateTutorialFree;
			GameStateUnload = GameStateTutorialUnload;
			break;
		case GS_Tutorial2:
			GameStateLoad = GameStateTutorial2Load;
			GameStateInit = GameStateTutorial2Init;
			GameStateUpdate = GameStateTutorial2Update;
			GameStateDraw = GameStateTutorial2Draw;
			GameStateFree = GameStateTutorial2Free;
			GameStateUnload = GameStateTutorial2Unload;
			break;
		default:	// Error handling
			AE_FATAL_ERROR("invalid state!!");
	}
}