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

// for fontID
#include "GameState_Mainmenu.hpp"

// for isPaused
#include "GameState_Platformer.hpp"

#include <iostream>

Player_stats player;
Checkpoint checkpoint[NUM_OF_CHECKPOINT] = { {0, 1550, 1650, 50, 150}, {0, 2500, 2600, -50, 50} };

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

	bullet_load();
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
	camera_speed = 15.0f;
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
	checkpoint_create(1550, 100, 0);
	checkpoint_create(2550, 0, 1);

	// -------- Printing out no. of lives --------
	if (player.Lives == 3) lives_counter = (s8*)"Lives: 3";
	else if (player.Lives == 2) lives_counter = (s8*)"Lives: 2";
	else if (player.Lives == 1) lives_counter = (s8*)"Lives: 1";
	else if (player.Lives == 0) lives_counter = (s8*)"YOU ARE DEAD!";

	// -------- Printing out Hp ----------
	if (player.Hp == 7) Hp = (s8*)"HP: 7";
	else if (player.Hp == 6) Hp = (s8*)"HP: 6";
	else if (player.Hp == 5) Hp = (s8*)"HP: 5";
	else if (player.Hp == 4) Hp = (s8*)"HP: 4";
	else if (player.Hp == 3) Hp = (s8*)"HP: 3";
	else if (player.Hp == 2) Hp = (s8*)"HP: 2";
	else if (player.Hp == 1) Hp = (s8*)"HP: 1";
	else if (player.Hp == 0) Hp = (s8*)"HP: 0";

	// --- Printing Level ---
	if (player.Level == 0) level = (s8*)"Level: 0";
	else if (player.Level == 1) level = (s8*)"Level: 1";
	else if (player.Level == 2) level = (s8*)"Level: 2";
	// --- Printing XP ---
	if (player.XP == 0) XP = (s8*)"XP: 0";
	else if (player.XP == 20) XP = (s8*)"XP: 20";
	else if (player.XP == 40) XP = (s8*)"XP: 40";

	AEGfxPrint(Albam_fontID, lives_counter, -1.0f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, level, -1.0f, 0.7f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, XP, -1.0f, 0.55f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, Hp, -0.15f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);
	

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
	//enemy_collision(&player); 
	enemy3_collision();
	//enemy_collision(&player, enemy1_a);
	//enemy_collision(&player, enemy1_b);

	// -------------  Camera   ---------------
	//AEGfxSetCamPosition(cameraX, cameraY);


	if (AEInputCheckTriggered(AEVK_B)) {
		free_moving_camera = !free_moving_camera;
	}

	if(free_moving_camera == false){
	//camera follow player's x
		//if (player.x > 0 && cameraPos.x < player.x) {
		//	cameraPos.x += (player.x - cameraPos.x)/camera_slowdown ;
		//}

		//else if ((player.x > 0 && cameraPos.x > player.x) || (player.x <= 0 && cameraPos.x > 0)) {
		//	cameraPos.x -=  (cameraPos.x - player.x) / camera_slowdown;
		//}


		////camera follow player's y
		//if (player.y > 0 && cameraPos.y < player.y) {
		//	cameraPos.y += (player.y - cameraPos.y)/camera_slowdown ;
		//}
		//else if ((player.y > 0 && cameraPos.y > player.y) || (player.y <= 0 && cameraPos.y > 0)) {
		//	cameraPos.y -= (cameraPos.y - player.y) / camera_slowdown;
		//}
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
			cameraPos.y += 2*camera_speed;

		if (AEInputCheckCurr(AEVK_K))
			cameraPos.y -= 2*camera_speed;

		if (AEInputCheckCurr(AEVK_J))
			cameraPos.x -= 2*camera_speed;

		if (AEInputCheckCurr(AEVK_L))
			cameraPos.x += 2*camera_speed;
	}
	//cameraPos= { player.x, player.y };
	//cameraPos.x = AEClamp(cameraPos.x, 0, player.x+AEGfxGetWinMaxX());		// If player.x < originX, cameraX = originX = 0		If player.x > originX, cameraX = player.x		If player.x > WINDOWX, cameraX = WINDOWX
	//std::cout << center_cursor.x << std::endl;
	//cameraPos.y = AEClamp(cameraPos.y,0 ,player.y+ AEGfxGetWinMaxY());		// If player.y < originY, cameraY = originY = 0		If player.y > originY, cameraY = player.y		If player.y > WINDOWY, cameraY = WINDOWY
	// Set camera pos
	// AEGfxSetCamPosition(cameraPos.x, cameraPos.y);

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
	if (player.x < -WINDOWLENGTH_X / 2 + PLAYER_WIDTH / 2)
		player.x = -WINDOWLENGTH_X / 2 + PLAYER_WIDTH / 2;

	// right of screen ---- CURRENTLY NO LIMIT ----
	//if (player.x > WINDOWXLENGTH / 2 - PLAYER_WIDTH / 2)
	//	player.x = WINDOWXLENGTH / 2 - PLAYER_WIDTH / 2;

	// top of screen
	if (player.y > (WINDOWLENGTH_Y / 2 - PLAYER_HEIGHT / 2) * 10)
		player.y = (WINDOWLENGTH_Y / 2 - PLAYER_HEIGHT / 2) * 10;

	// bottom of screen
	if (player.y < -WINDOWLENGTH_Y / 2 + PLAYER_HEIGHT / 2) {
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
