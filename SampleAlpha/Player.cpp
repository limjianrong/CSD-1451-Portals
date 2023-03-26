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
#include <fstream>
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
float camera_speed{};
// ----- Camera -----
extern bool isPaused;
// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern f32 originX, originY;			// Center of screen, no matter where the camera moves

AEVec2 cameraPos{ 0, 0 };
float constexpr camera_buffer_range{ 200.0f };
// ----- Enemy -----
//extern Enemy1_stats enemy1_a, enemy1_b;

//---------File IO-------
std::ifstream player_ifs{};

void player_load() {
	player.player_standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_stand.png");
	player.player_left1Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk1_left.png");
	player.player_left2Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk2_left.png");
	player.player_right1Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk1_right.png");
	player.player_right2Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk2_right.png");
	checkpoint[0].checkpointTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/cactus.png");

	player.fullLivesTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/redCrystal.png");
	//player.fullLivesTex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Items/platformPack_item017.png");
	player.emptyLivesTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/outlineCrystal.png");
	player_ifs.open("Assets/textFiles/player_stats.txt");
	if (!player_ifs) {
		std::cout << "\nFailed to open player_stats.txt";
	}
	// -------- Player --------
	std::string str{};
	player_ifs >> str >> player.width;				// Player's width
	player_ifs >> str >> player.height;			// Player's height
	player_ifs >> str >> player.initial_pos_x;		// Player's initial X position
	player_ifs >> str >> player.initial_pos_y;		// Player's initial Y position
	player_ifs >> str >> player.highest_level;		// Level cap of 30 lvls
	player_ifs >> str >> player.XP_TILL_10;		// 40 XP to level up for lvls 0-10
	player_ifs >> str >> player.XP_TILL_20;		// 100 XP to level up for lvls 10-20
	player_ifs >> str >> player.XP_TILL_30;		// 160 XP to level up for lvls 20-30
	player_ifs >> str >> player.XP_RESET;			// Reset XP to 0
	//ifs >> str >> player.rotation;			// Player's Rotation
	//ifs >> str >> player.Max_Hp;			// Player's Maximum Health
	//ifs >> str >> player.Hp;				// Player's Maximum Health
	//ifs >> str >> player.Max_Hp_Reset;		// Player's Maximum Health
	//ifs >> str >> player.Lives_Reset;		// Player's Lives
	//ifs >> str >> player.Speed_Reset;		// Player's Movement Speed
	//ifs >> str >> player.Level_Reset;		// Player's Level
	//ifs >> str >> player.XP_RESET;			// Player's XP
	//ifs >> str >> player.justLeveledUp;		// Indicator to show player levelling up

	/*rotation 0
	Max_Hp 5
	Hp 5
	Max_Hp_Reset 5
	Lives_Reset 2
	Speed_Reset 1
	Level_Reset 0
	XP_Reset 0
	justLeveledUp 0*/

	player_ifs.close();

}

void player_init() {


	//player.x				= player.initial_pos_x;		// Player's initial X position
	//player.y				= player.initial_pos_y;		// Player's initial Y position
	////player.rotation			= 0.f;					// Player's Rotation
	//player.Hp				= player.Max_Hp_Reset;		// Player's Health
	//player.Max_Hp			= player.Max_Hp_Reset;		// Player's Maximum Health
	//player.Lives			= player.Lives_Reset;		// Player's Lives
	//player.Speed			= player.Speed_Reset;		// Player's Movement Speed
	//player.Level			= player.Level_Reset;		// Player's Level
	//player.XP				= player.XP_RESET;			// Player's XP
	//player.justLeveledUp	= FALSE;					// Indicator to show player levelling up


	player.x				= player.initial_pos_x;		// Player's initial X position
	player.y				= player.initial_pos_y;		// Player's initial Y position
	player.rotation			= 0.f;						// Player's Rotation
	player.Max_Hp			= 5;						// Player's Maximum Health
	player.Hp				= player.Max_Hp;			// Player's Health
	player.Lives			= 3;						// Player's Lives
	player.Speed			= 1;						// Player's Movement Speed
	player.Level			= 0;						// Player's Level
	player.XP				= 0;						// Player's XP
	player.justLeveledUp	= FALSE;					// Indicator to show player levelling up


	player.Lives_height		= 50.0f;
	player.Lives_width		= 50.0f;
	//player.Lives_x = 0;
	//player.Lives_y = 0;

	// -------- Camera --------
	cameraPos.x = 0;
	cameraPos.y = 0; // Reset camera
	free_moving_camera = false;
	camera_speed = 30.0f;
	// -------- Checkpoint --------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		checkpoint[i].check = FALSE;					// Disable all checkpoints
	}

	// -------- Pause Menu --------
	isPaused = FALSE;		// Unpause game

}

void player_draw() {

	
	// ---------------- Player ----------------
	// Creates a player size 50x50
	AEMtx33Scale(&player.scale, player.width, player.height);
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

	/*std::string lives_string = "Lives: ";
	AEGfxPrint(Albam_fontID, &lives_string[0], -1.0f, 0.85f, 1, 0.0f, 0.0f, 0.0f);
	std::string lives_counter_string = std::to_string(player.Lives);
	AEGfxPrint(Albam_fontID, &lives_counter_string[0], -0.70f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);*/

	// --- 1st Life ---
	AEMtx33Scale(&player.scale, player.Lives_width, player.Lives_height);
	AEMtx33Rot(&player.rotate, PI);
	AEMtx33Trans(&player.translate, AEGfxGetWinMinX() + 30.0f, AEGfxGetWinMaxY() - 25.0f);
	AEMtx33Concat(&player.transform, &player.rotate, &player.scale);
	AEMtx33Concat(&player.transform, &player.translate, &player.transform);
	AEGfxSetTransform(player.transform.m);
	if (player.Lives >= 1)
		AEGfxTextureSet(player.fullLivesTex, 0, 0);
	else
		AEGfxTextureSet(player.emptyLivesTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// --- 2nd Life ---
	AEMtx33Trans(&player.translate, AEGfxGetWinMinX() + 90.0f, AEGfxGetWinMaxY() - 25.0f);
	AEMtx33Concat(&player.transform, &player.rotate, &player.scale);
	AEMtx33Concat(&player.transform, &player.translate, &player.transform);
	AEGfxSetTransform(player.transform.m);
	if (player.Lives >= 2)
		AEGfxTextureSet(player.fullLivesTex, 0, 0);
	else
		AEGfxTextureSet(player.emptyLivesTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// --- 3rd Life ---
	AEMtx33Trans(&player.translate, AEGfxGetWinMinX() + 150.0f, AEGfxGetWinMaxY() - 25.0f);
	AEMtx33Concat(&player.transform, &player.rotate, &player.scale);
	AEMtx33Concat(&player.transform, &player.translate, &player.transform);
	AEGfxSetTransform(player.transform.m);
	if (player.Lives >= 3)
		AEGfxTextureSet(player.fullLivesTex, 0, 0);
	else
		AEGfxTextureSet(player.emptyLivesTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

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
	if (AEInputCheckCurr(AEVK_J)) {
		player.Max_Hp = 10;						// Player's Maximum Health
		player.Hp = player.Max_Hp;			// Player's Health
		std::cout << player.Max_Hp << std::endl;
	}
	// ---------  Player's movement   -----------
	// D key pressed
	if (AEInputCheckCurr(AEVK_D)) {
		player.x += 5 * player.Speed;
		num_of_Dpressed++;
		if (player.x > ((AEGfxGetWinMinX() + AEGfxGetWinMaxX())/2 + camera_buffer_range) && free_moving_camera == false) {
			cameraPos.x += 5 * player.Speed;
		}
		//player.rotation -= 0.1f;
	}
	// A key pressed
	else if (AEInputCheckCurr(AEVK_A)) {
		player.x -= 5 * player.Speed;
		num_of_Apressed++;
		if (player.x < ((AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2 - camera_buffer_range) && free_moving_camera == false) {
			cameraPos.x -= 5 * player.Speed;
		}
	}

	// --------  Player's level & XP   ----------
	// 40xp to level up for lvls 0-10 (1 enemy = 20xp)
	if (player.XP == player.XP_TILL_10 && player.Level >= 0 && player.Level <= 10) {
		player.Level++;
		player.XP = player.XP_RESET;
		player.justLeveledUp = TRUE;
	}
	// 100xp to level up for lvls 10-20 (1 enemy = 20xp)
	else if (player.XP == player.XP_TILL_20 && player.Level >= 10 && player.Level <= 20) {
		player.Level++;
		player.XP = player.XP_RESET;
		player.justLeveledUp = TRUE;
	}
	// 160xp to level up for lvls 20-30 (1 enemy = 20xp)
	else if (player.XP == player.XP_TILL_30 && player.Level >= 20 && player.Level <= 30) {
		player.Level++;
		player.XP = player.XP_RESET;
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
		//if previous camera state was free_moving(for level-design), and B was pressed
		//switch back to camera state that is used for playing, now the camera goes back
		//to following the player
		if (!free_moving_camera) {
			cameraPos.x = player.x;
			cameraPos.y = player.y;
		}
	}

	if(free_moving_camera == false){
		//check player input for camera reset
		if (AEInputCheckTriggered(AEVK_G)) {
			if (player.x < 0) {
				cameraPos.x = 0;
			}
			else {
				cameraPos.x = player.x;
			}
			if (player.y < 0) {
				cameraPos.y = 0;
			}
			else {
				cameraPos.y = player.y;
			}
		}

		if (player.x > AEGfxGetWinMaxX()) {
			cameraPos.x = player.x;
		}

		else if (player.x < AEGfxGetWinMinX()) {
			if (player.x < 0) {
				cameraPos.x = 0;
			}
			else {
				cameraPos.x = player.x;
			}
		}

		//camera will always follow player's y if player.y is +ve
		if (player.y > 0) {
			cameraPos.y = player.y;
		}

		//if player.y is -ve, camera.y stays fixed at 0
		else if (player.y < 0) {
			cameraPos.y = 0;
		}

	}
	//controls for free moving camera, used for debugging/level design purposes
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
	// Unload player sprites textures
	AEGfxTextureUnload(player.player_standTex);
	AEGfxTextureUnload(player.player_left1Tex);
	AEGfxTextureUnload(player.player_right1Tex);
	AEGfxTextureUnload(player.player_left2Tex);
	AEGfxTextureUnload(player.player_right2Tex);
	AEGfxTextureUnload(checkpoint[0].checkpointTex);

	// Unload Lives (Heart) texture
	AEGfxTextureUnload(player.fullLivesTex);
	AEGfxTextureUnload(player.emptyLivesTex);

}

void player_collision() {


	// left of screen
	if (player.x < -WINDOWLENGTH_X / 2.f + player.width / 2.f)
		player.x = -WINDOWLENGTH_X / 2.f + player.width / 2.f;

	// right of screen ---- CURRENTLY NO LIMIT ----

	// top of screen
	if (player.y > (WINDOWLENGTH_Y / 2.f - player.height / 2.f) * 10)
		player.y = (WINDOWLENGTH_Y / 2.f - player.height / 2.f) * 10;

	// bottom of screen
	if (player.y < -WINDOWLENGTH_Y / 2.f + player.height / 2.f) {
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
				player.x = player.initial_pos_x;
				player.y = player.initial_pos_y;
			}
		}
		if (player.x > 0) {
			cameraPos.x = player.x;
		}
		else {
			cameraPos.x = 0;
		}
		cameraPos.y = player.y;
	}
}

void checkpoint_create(f32 x, f32 y, s32 index) {
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; ++i) {
		AEMtx33Scale(&checkpoint[i].scale, player.width * 2, player.height * 2);
		AEMtx33Rot(&checkpoint[i].rotate, PI);
		AEMtx33Trans(&checkpoint[i].translate, x, y);
		AEMtx33Concat(&checkpoint[i].transform, &checkpoint[i].rotate, &checkpoint[i].scale);
		AEMtx33Concat(&checkpoint[i].transform, &checkpoint[i].translate, &checkpoint[i].transform);
		AEGfxSetTransform(checkpoint[i].transform.m);
		AEGfxTextureSet(checkpoint[0].checkpointTex, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
