/******************************************************************************/
/*!
\file		GameStateList.hpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	30 January, 2023
\brief		Header file containing the enums of the different states

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


// ---------------------------------------------------------------------------
// game state list

#ifndef GAM_GAME_STATE_LIST_H_
#define GAM_GAME_STATE_LIST_H_
enum
{
	// list of all game states 
	GS_MainMenu = 0,
	GS_Platformer,
	GS_Settings,
	GS_Win,
	GS_Lose,
	// special game state. Do not change
	GS_RESTART,
	GS_QUIT,
	GS_NONE
};

// ---------------------------------------------------------------------------
#endif