 /*==================================================================================
 * All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
 * File:				GameState_Credits.hpp
 * Course:				CSD1451
 * Group Name:			Apparate
 * Primary Author:		Tay Zhun Hang (zhunhang.tay@digipen.edu)
 *
 * Brief:
	This header file contains the declaration of functions that implements
	the credits
 ==================================================================================*/
#define NUM 4
#define INCREMENT 1.f

// struct for credit
struct credit_stat {
	AEGfxTexture* texture;
	f32 y;
};

// main functions for this game state
void GameStateCreditsLoad(void);
void GameStateCreditsInit(void);
void GameStateCreditsUpdate(void);
void GameStateCreditsDraw(void);
void GameStateCreditsFree(void);
void GameStateCreditsUnload(void);
