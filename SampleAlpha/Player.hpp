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
void player_collision();

// -----  Player's width & height (Adjustable)  -------
#define PLAYER_WIDTH 50.0f
#define PLAYER_HEIGHT 50.0f

// --------------  Initialise player  ----------------
struct Player_stats {
	f32 x{ -450.0f }, y{ -100.0f }, rotation{ 0.0f };
	s32 Hp, Lives;
};

void endpoint_init(void);
void check_endpoint(f32 playerx, f32 playery, AEGfxVertexList* endpoint_rectangle,
	AEGfxVertexList* endpoint_triangle, AEVec2* PlayerCenter, AEVec2& endpoint_center);


