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
void checkpoint_create(f32 x, f32 y);


// -----  Player's width & height (Adjustable)  -------
#define PLAYER_WIDTH 50.0f
#define PLAYER_HEIGHT 50.0f

// --------------  Initialise player  ----------------
struct Player_stats {
	f32 x{ -450.0f }, y{ -100.0f }, rotation{ 0.0f };
	s32 Hp{ 5 }, Lives{ 3 };
};

struct Checkpoint {
	bool check;
	f32 x1, x2, y1, y2;
};



