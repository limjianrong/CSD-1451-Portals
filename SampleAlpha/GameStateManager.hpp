/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameStateManager.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This header file contains definitions of functions that initialize the gameState and updates the gameState

  The function includes:
  - GameStateMgrInit
	Initialise the gameState variables

  - GameStateMgrUpdate
	Updates the gameState based on current input
==================================================================================*/
#pragma once
#include "AEEngine.h"
#include "GameStateList.hpp"

typedef void(*FP)(void);

extern u32 gGameStateInit;
extern u32 gGameStateCurr;
extern u32 gGameStatePrev;
extern u32 gGameStateNext;

extern FP GameStateLoad, GameStateInit, GameStateUpdate, GameStateDraw, GameStateFree, GameStateUnload;

// ---------------------------------------------------------------------------
// Function prototypes

// call this at the beginning and AFTER all game states are added to the manager
void GameStateMgrInit(u32 gameStateInit);

// update is used to set the function pointers
void GameStateMgrUpdate();

// ---------------------------------------------------------------------------