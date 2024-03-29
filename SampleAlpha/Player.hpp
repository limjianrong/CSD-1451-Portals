 /*==================================================================================
 * All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
 * File:				Player.hpp
 * Course:				CSD1451
 * Group Name:			Apparate
 * Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
 * Secondary Authors:	-
 *
 * Brief:
   This header file contains definition of functions for player related logic
   Contains definition of derived class struct with player's specific attribute and
   struct class for checkpoints. Define functions that updates player's level based
   on XP, player's movement, player's collision and checkpoints within the game
 ==================================================================================*/

#pragma once
#include "Utilities.hpp"

#define FIXED_MOVEMENT 5

#define max_Dpresses 9
#define range_Dpresses 4
#define max_AudioDelay 19

#define LEVEL_SYSTEM_0 0
#define LEVEL_SYSTEM_1 5
#define LEVEL_SYSTEM_2 10
#define LEVEL_SYSTEM_3 15

#define Lives_OffsetY 50
#define XP_BAR_HEIGHT 50
#define XP_BAR_OFFSETX 160

#define CHECKPOINT_WIDTH 50
#define CHECKPOINT_HEIGHT 50

void player_load();
void player_init();
void player_draw();
void player_update();
void player_unload();

void Render_LevelnXP();
void level_xp_system_update(s32 requiredXP, s32 levelSystem_LowerRange, s32 levelSystem_HigherRange);
void Render_Lives(s32 currLife, f32 offsetX);
void respawn_player();
void player_collision();


// --------------  Initialise player  ----------------
struct Player_stats : GameObjects {
	s32 initial_pos_x, initial_pos_y; //initial start point of player

	// -----  Player's levelling requirements  -------
	s32 XP_TILL_SYSTEM1, XP_TILL_SYSTEM2, XP_TILL_SYSTEM3, XP_RESET;
	s32 highest_level; // Highest level a player can level up to

	// ----- Player's attributes -----
	s32 Lives, Lives_Reset;
	f32 Speed, Speed_Reset;
	s32 Level, Level_Reset, XP, requiredXP;

	AEVec2 Lives_dimensions, Lives_pos;
	bool justLeveledUp, isShieldActive;

	AEVec2 bottom_hotspot;
	// ----- Transformation & Texture -----
	AEGfxTexture* player_right1Tex, * player_right2Tex, * player_left1Tex, * player_left2Tex, * player_standTex;
	AEGfxTexture* fullLivesTex, * emptyLivesTex;

	// ----- Audio -----
	AEAudio walkAudio, levelUpAudio;
};

// --------------  Checkpoints  ----------------
struct Checkpoint {
	bool check;
	f32 x1, x2, y1, y2;

	// ----- Transformation & Texture -----
	AEMtx33 scale, rotate, translate, transform;
	AEGfxTexture* checkpointTex;

	// ----- Audio -----
	AEAudio checkpointAudio;
};
#define NUM_OF_CHECKPOINT 5
void checkpoint_create(f32 x, f32 y, s32 index);



