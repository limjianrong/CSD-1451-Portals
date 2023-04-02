/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameStateList.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  Header file containing the enums of the different states
==================================================================================*/


// ---------------------------------------------------------------------------
// game state list

#ifndef GAM_GAME_STATE_LIST_H_
#define GAM_GAME_STATE_LIST_H_
enum
{
	// list of all game states 
	GS_MainMenu = 0,
	GS_Platformer,
	GS_Win,
	GS_Lose,
	GS_SplashScreen,
	GS_Credits,
	// special game state. Do not change
	GS_RESTART,
	GS_QUIT,
	GS_NONE
};

// ---------------------------------------------------------------------------
#endif