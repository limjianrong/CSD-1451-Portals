/******************************************************************************//*!
\file		Player.cpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	1 February, 2023
\brief		Source file for the platformer game state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#include "AEEngine.h"

#include "Player.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "Utilities.hpp"
#include "draw_level.hpp"
#include "Enemy.hpp"
#include "Enemy3.hpp"
#include <string>

// for fontID
#include "GameState_Mainmenu.hpp"

// for isPaused
#include "GameState_Platformer.hpp"

#include <iostream>

Player_stats player;
Checkpoint checkpoint[NUM_OF_CHECKPOINT] = { {0, 950, 1050, 400, 500}, {0, 3500, 3600, 900, 1000} , {0, 7050, 7150, 800, 900} , 
												{0, 2600, 2700, -200, -100} , {0, 7050, 6950, 300, 500} };

// ------  Text  ------
extern s8 Albam_fontID;
s8* lives_counter; // temp counter (Replacing with hearts?)
s8* level, * XP, * Hp;
// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh

int num_of_Apressed{ 0 }, num_of_Dpressed{ 0 };
bool free_moving_camera{};
float camera_speed{}, camera_slowdown{};
// ----- Camera -----
extern bool isPaused;
// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern f32 originX, originY;			// Center of screen, no matter where the camera moves

AEVec2 cameraPos{ 0, 0 };
// ----- Enemy -----
//extern Enemy1_stats enemy1_a, enemy1_b;

void player_load() {
	player.player_standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_stand.png");
	player.player_left1Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk1_left.png");
	player.player_left2Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk2_left.png");
	player.player_right1Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk1_right.png");
	player.player_right2Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk2_right.png");
	checkpoint[0].checkpointTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/cactus.png");
}

void player_init() {
	// -------- Player --------
	player.x				= PLAYER_INITIAL_POS_X;		// Player's initial X position
	player.y				= PLAYER_INITIAL_POS_Y;		// Player's initial Y position
	player.rotation			= 0.f;						// Player's Rotation
	player.Max_Hp			= 5;						// Player's Maximum Health
	player.Hp				= player.Max_Hp;			// Player's Health
	player.Lives			= 3;						// Player's Lives
	player.Speed			= 1;						// Player's Movement Speed
	player.Level			= 0;						// Player's Level
	player.XP				= 0;						// Player's XP
	player.justLeveledUp	= FALSE;					// Indicator to show player levelling up

	// -------- Camera --------
	AEGfxSetCamPosition(0, 0);							// Reset camera
	free_moving_camera = false;
	camera_speed = 30.0f;
	camera_slowdown = 10.0f;
	// -------- Checkpoint --------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		checkpoint[i].check = FALSE;					// Disable all checkpoints
	}

	// -------- Pause Menu --------
	isPaused = FALSE;									// Unpause game
}

void player_draw() {

	
	// ---------------- Player ----------------
	// Creates a player size 50x50
	AEMtx33Scale(&player.scale, PLAYER_WIDTH, PLAYER_HEIGHT);
	// Rotate player
	AEMtx33Rot(&player.rotate, PI);
	// Move player when A / D keys pressed
	AEMtx33Trans(&player.translate, player.x, player.y);
	// Concat the matrices (TRS)
	AEMtx33Concat(&player.transform, &player.rotate, &player.scale);
	AEMtx33Concat(&player.transform, &player.translate, &player.transform);
	// Choose the transform to use
	AEGfxSetTransform(player.transform.m);
	if (AEInputCheckCurr(AEVK_D)) {
		if ((num_of_Dpressed % 9) <= 4) AEGfxTextureSet(player.player_left1Tex, 0, 0);
		else if ((num_of_Dpressed % 9) >= 5) AEGfxTextureSet(player.player_left2Tex, 0, 0);
	}
	else if (AEInputCheckCurr(AEVK_A)) {
		if ((num_of_Apressed % 9) <= 4) AEGfxTextureSet(player.player_right1Tex, 0, 0);
		else if ((num_of_Apressed % 9) >= 5) AEGfxTextureSet(player.player_right2Tex, 0, 0);
	} 
	else AEGfxTextureSet(player.player_standTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// -------------- Checkpoint --------------
	checkpoint_create(1000, 450, 0);// y+100
	checkpoint_create(3550, 950, 1);//6, 3350, 200, 11
	checkpoint_create(7100, 850, 2);//4, 7000, 750, 8
	checkpoint_create(2650, -150, 3);//(4, 2550, -250, 10);
	checkpoint_create(7000, 400, 4);//(7, 6900, 300, 14);

	// -------- Printing out no. of lives --------
	std::string lives_string = "Lives: ";
	AEGfxPrint(Albam_fontID, &lives_string[0], -1.0f, 0.85f, 1, 0.0f, 0.0f, 0.0f);
	std::string lives_counter_string = std::to_string(player.Lives);
	AEGfxPrint(Albam_fontID, &lives_counter_string[0], -0.70f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);

	// -------- Printing out Hp ----------
	std::string hp_string = "Hp: ";
	AEGfxPrint(Albam_fontID, &hp_string[0], -0.05f, 0.85f, 1, 0.0f, 0.0f, 0.0f);
	std::string hp_counter_string = std::to_string(player.Hp);
	AEGfxPrint(Albam_fontID, &hp_counter_string[0], 0.15f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);

	// --- Printing Level ---
	std::string level_string = "Level: ";
	AEGfxPrint(Albam_fontID, &level_string[0], -1.0f, 0.70f, 1, 0.0f, 0.0f, 0.0f);
	std::string level_counter_string = std::to_string(player.Level);
	AEGfxPrint(Albam_fontID, &level_counter_string[0], -0.7f, 0.70f, 1.0f, 0.0f, 0.0f, 0.0f);

	// --- Printing XP ---
	std::string xp_string = "Xp: ";
	AEGfxPrint(Albam_fontID, &xp_string[0], -1.0f, 0.55f, 1, 0.0f, 0.0f, 0.0f);
	std::string xp_counter_string = std::to_string(player.XP);
	AEGfxPrint(Albam_fontID, &xp_counter_string[0], -0.7f, 0.55f, 1.0f, 0.0f, 0.0f, 0.0f);

	

	// ---------  Firing of bullets   -----------
	/*if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(player.x, player.y, 1);
	}
	else {
		weapon_fire(player.x, player.y, 0);
	}*/
}

void player_update() {
	// --------  Setting player's position into a vector --------
	AEVec2Set(&player.center, player.x, player.y);

	// ---------  Player's movement   -----------
	// D key pressed
	if (AEInputCheckCurr(AEVK_D)) {
		player.x += 5 * player.Speed;
		num_of_Dpressed++;
		//player.rotation -= 0.1f;
	}
	// A key pressed
	else if (AEInputCheckCurr(AEVK_A)) {
		player.x -= 5 * player.Speed;
		num_of_Apressed++;
		//player.rotation += 0.1f;
	}
	




	//update player's bottom hotspot
	//AEVec2Set(&player.bottom_hotspot, player.x, player.y - player.height / 2);
	// --------  Player's level & XP   ----------
	// FOR NOW ONLY: 20xp to level up from lvl0 -> lvl1 (1 enemy = 20xp)
	if (player.XP == 20 && player.Level == 0) {
		player.Level++;
		player.XP = 0;
		player.justLeveledUp = TRUE;
	}
	// FOR NOW ONLY: 40xp to level up from lvl1 -> lvl2 (1 enemy = 20xp)
	else if (player.XP == 40 && player.Level == 1) {
		player.Level++;
		player.XP = 0;
		player.justLeveledUp = TRUE;
	}
	// FOR NOW ONLY: 60xp to level up from lvl2 -> lvl3 (1 enemy = 20xp)
	else if (player.XP == 60 && player.Level == 2) {
		player.Level++;
		player.XP = 0;
		player.justLeveledUp = TRUE;
	}

	// --------    Player Hp & Lives   -----------
	if (player.Hp <= 0) {
		player.Lives--;
		player.Hp = player.Max_Hp;
	}

	// ---------  Firing of bullets   -----------
	/*if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(player.x, player.y, 1);
	}
	else {
		weapon_fire(player.x, player.y, 0);
	}*/



	// ------------  Collision   --------------
	player_collision();
	enemy3_collision();

	// -------------  Camera   ---------------
	//AEGfxSetCamPosition(cameraX, cameraY);


	if (AEInputCheckTriggered(AEVK_B)) {
		free_moving_camera = !free_moving_camera;
	}

	if(free_moving_camera == false){
	//camera follow player's x
		if (player.x > 0) {
			cameraPos.x = player.x;
		}
		else if (player.x <= 0) {
			cameraPos.x = 0;
		}

		if (player.y > 0) {
			cameraPos.y = player.y;
		}
		else if (player.x <= 0) {
			cameraPos.y = 0;
		}
	}

	if (free_moving_camera == true) {
		if (AEInputCheckCurr(AEVK_I))
			cameraPos.y += camera_speed;

		if (AEInputCheckCurr(AEVK_K))
			cameraPos.y -= camera_speed;

		if (AEInputCheckCurr(AEVK_J))
			cameraPos.x -= camera_speed;

		if (AEInputCheckCurr(AEVK_L))
			cameraPos.x += camera_speed;
	}

	// -------------  Update latest checkpoint for player  -------------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		if (player.x >= checkpoint[i].x1 && player.x <= checkpoint[i].x2 &&
			player.y >= checkpoint[i].y1 && player.y <= checkpoint[i].y2) {
			checkpoint[i].check = TRUE;
			//checkpoint[i-1].check = 0;    //-----> If player position updates according to most recent checkpoint & NOT furthest checkpoint
		}
	}
}

void player_unload() {
	AEGfxTextureUnload(player.player_standTex);
	AEGfxTextureUnload(player.player_left1Tex);
	AEGfxTextureUnload(player.player_right1Tex);
	AEGfxTextureUnload(player.player_left2Tex);
	AEGfxTextureUnload(player.player_right2Tex);
	AEGfxTextureUnload(checkpoint[0].checkpointTex);
}

void player_collision() {


	// left of screen
	if (player.x < -WINDOWLENGTH_X / 2.f + PLAYER_WIDTH / 2.f)
		player.x = -WINDOWLENGTH_X / 2.f + PLAYER_WIDTH / 2.f;

	// right of screen ---- CURRENTLY NO LIMIT ----

	// top of screen
	if (player.y > (WINDOWLENGTH_Y / 2.f - PLAYER_HEIGHT / 2.f) * 10)
		player.y = (WINDOWLENGTH_Y / 2.f - PLAYER_HEIGHT / 2.f) * 10;

	// bottom of screen
	if (player.y < -WINDOWLENGTH_Y / 2.f + PLAYER_HEIGHT / 2.f) {
		--player.Lives;
		player.Hp = player.Max_Hp;

		// ---------  Set player's position to latest checkpoint  ---------
		for (s32 i = NUM_OF_CHECKPOINT-1; i >= 0; i--) {
			if (checkpoint[i].check) {
				player.x = checkpoint[i].x1 + 50;
				player.y = checkpoint[i].y1;
				break;
			}
			else {
				player.x = PLAYER_INITIAL_POS_X;
				player.y = PLAYER_INITIAL_POS_Y;
			}
		}
	}
}

void checkpoint_create(f32 x, f32 y, s32 index) {
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; ++i) {
		AEMtx33Scale(&checkpoint[i].scale, PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2);
		AEMtx33Rot(&checkpoint[i].rotate, PI);
		AEMtx33Trans(&checkpoint[i].translate, x, y);
		AEMtx33Concat(&checkpoint[i].transform, &checkpoint[i].rotate, &checkpoint[i].scale);
		AEMtx33Concat(&checkpoint[i].transform, &checkpoint[i].translate, &checkpoint[i].transform);
		AEGfxSetTransform(checkpoint[i].transform.m);
		AEGfxTextureSet(checkpoint[0].checkpointTex, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
