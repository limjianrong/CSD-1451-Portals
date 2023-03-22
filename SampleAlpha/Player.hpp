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
void player_load();
void player_init();
void player_draw();
void player_update();
void player_unload();


void player_collision();


// -----  Player's levelling requirements  -------
//#define PLAYER_NEEDED_XP_TILL_10 40
//#define PLAYER_NEEDED_XP_TILL_20 100
//#define PLAYER_NEEDED_XP_TILL_30 160
//#define PLAYER_XP_RESET			 0

// --------------  Initialise player  ----------------
struct Player_stats {
	s32 initial_pos_x{}; //initial start point of player
	s32 initial_pos_y{}; //initial start point of player
	s32 highest_level{}; //highest level a player can level up to
	s32 XP_TILL_10{}, XP_TILL_20{}, XP_TILL_30{}, XP_RESET{};

	f32 x, y, rotation;
	AEVec2 center{ 0,0 }; // same as .x & .y
	f32 width{ }, height{};
	s32 Max_Hp, Hp, Lives;
	f32 Speed;
	s32 Level, XP, requiredXP;
	bool justLeveledUp;

	AEVec2 bottom_hotspot{};
	// ----- Transformation & Texture -----
	AEMtx33 scale, rotate, translate, transform;
	AEGfxTexture* player_right1Tex, * player_right2Tex, * player_left1Tex, * player_left2Tex, * player_standTex;
};

// --------------  Checkpoints  ----------------
struct Checkpoint {
	bool check;
	f32 x1, x2, y1, y2;

	// ----- Transformation & Texture -----
	AEMtx33 scale, rotate, translate, transform;
	AEGfxTexture* checkpointTex;
};
#define NUM_OF_CHECKPOINT 5
void checkpoint_create(f32 x, f32 y, s32 index);



