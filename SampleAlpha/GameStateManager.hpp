/*!**************************************************************************************************
\file     GameStateManager.hpp
\author   Lin ZhaoZhi
\par      DP email: z.lin@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file contains definitions of functions that initialize the gameState and updates the gameState

  The function includes:
  - GameStateMgrInit
	Initialise the gameState variables

  - GameStateMgrUpdate
	Updates the gameState based on current input
*****************************************************************************************************/
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