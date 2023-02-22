/******************************************************************************//*!
\file		Player.hpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	1 February, 2023
\brief		Source file for the platformer game state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#pragma once
void initialize_player();
void draw_player();
void update_player();
void unload_player();


void player_collision();


// -----  Player's width & height (Adjustable)  -------
#define PLAYER_WIDTH 50.0f
#define PLAYER_HEIGHT 50.0f
// -----  Player's initial spawn position (Adjustable)  -------
#define PLAYER_INITIAL_POS_X -450
#define PLAYER_INITIAL_POS_Y -100

// --------------  Initialise player  ----------------
struct Player_stats {
	f32 x{ PLAYER_INITIAL_POS_X }, y{ PLAYER_INITIAL_POS_Y }, rotation{ 0.0f };
	s32 Hp{ 5 }, Lives{ 3 };
	s32 Level{ 0 }, XP{ 0 };
	AEVec2 center{ 0,0 };
};

// --------------  Checkpoints  ----------------
struct Checkpoint {
	bool check;
	f32 x1, x2, y1, y2;
};
#define NUM_OF_CHECKPOINT 2
void checkpoint_create(f32 x, f32 y);



