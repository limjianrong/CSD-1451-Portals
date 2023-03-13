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


// -----  Player's width & height (Adjustable)  -------
#define PLAYER_WIDTH 50.0f
#define PLAYER_HEIGHT 50.0f
// -----  Player's initial spawn position (Adjustable)  -------
#define PLAYER_INITIAL_POS_X -450
#define PLAYER_INITIAL_POS_Y -100

//#define PLAYER_INITIAL_POS_X 2250
//#define PLAYER_INITIAL_POS_Y 550

// --------------  Initialise player  ----------------
struct Player_stats {
	f32 x, y, rotation;
	AEVec2 center{ 0,0 }; // same as .x & .y
	f32 width{ PLAYER_WIDTH }, height{ PLAYER_HEIGHT };
	s32 Max_Hp, Hp, Lives;
	f32 Speed;
	s32 Level, XP;
	bool justLeveledUp;
	bool falling{false};
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
#define NUM_OF_CHECKPOINT 2
void checkpoint_create(f32 x, f32 y, s32 index);



