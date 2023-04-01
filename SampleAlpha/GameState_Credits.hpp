#pragma once
/******************************************************************************//*!
\file		GameState_Credits.hpp
\author 	Digipen, Tay Zhun Hang
\par    	email: zhunhang.tay@digipen.edu
\date   	26 March, 2023
\brief

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/

enum { APPEAR, DISAPPEAR };

struct credit_stat {
	AEGfxTexture* texture;
	f32 trans{1.f};
	int flag = APPEAR;
};

void GameStateCreditsLoad(void);
void GameStateCreditsInit(void);
void GameStateCreditsUpdate(void);
void GameStateCreditsDraw(void);
void GameStateCreditsFree(void);
void GameStateCreditsUnload(void);

//draw functions for the different credit pictures
void draw_credit_1();
void draw_credit_2();
void draw_credit_3();
void draw_credit_4();