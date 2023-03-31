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
#include "camera.hpp"
//#include "Audio.hpp"
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
// --- Audio ---
extern AEAudioGroup soundGroup;

int num_of_Apressed{ 0 }, num_of_Dpressed{ 0 };

extern bool isPaused;
// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern AEVec2 origin;					// Center of screen, no matter where the camera moves

// ----- Camera -----
extern Camera camera;

// ----- Enemy -----
extern Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
extern Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2

//---------File IO-------
std::ifstream player_ifs{};

void player_load() {
	player.player_standTex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	player.player_left1Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	player.player_left2Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	player.player_right1Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	player.player_right2Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	checkpoint[0].checkpointTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/other/flagGreen_up.png");

	player.fullLivesTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/redCrystal.png");
	//player.fullLivesTex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Items/platformPack_item017.png");
	player.emptyLivesTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/outlineCrystal.png");
	player_ifs.open("Assets/textFiles/player_stats.txt");
	if (!player_ifs) {
		std::cout << "\nFailed to open player_stats.txt";
	}
	// -------- Player --------
	std::string str{};
	player_ifs >> str >> player.dimensions.x;				// Player's width
	player_ifs >> str >> player.dimensions.y;				// Player's height
	player_ifs >> str >> player.initial_pos_x;		// Player's initial X position
	player_ifs >> str >> player.initial_pos_y;		// Player's initial Y position
	player_ifs >> str >> player.highest_level;		// Level cap of 30 lvls

	player_ifs >> str >> player.XP_TILL_10;			// 40 XP to level up for lvls 0-10
	player_ifs >> str >> player.XP_TILL_20;			// 100 XP to level up for lvls 10-20
	player_ifs >> str >> player.XP_TILL_30;			// 160 XP to level up for lvls 20-30
	player_ifs >> str >> player.XP_RESET;			// Reset XP to 0

	player_ifs >> str >> player.Max_Hp;				// Player's Maximum Health
	player_ifs >> str >> player.Hp;					// Player's Maximum Health
	player_ifs >> str >> player.Max_Hp_Reset;		// Player's Maximum Health Reset
	player_ifs >> str >> player.Lives_Reset;		// Player's Lives Reset
	player_ifs >> str >> player.Speed_Reset;		// Player's Movement Speed Reset
	player_ifs >> str >> player.Level_Reset;		// Player's Level Reset
	player_ifs >> str >> player.XP_RESET;			// Player's XP Reset
	player_ifs >> str >> player.justLeveledUp;		// Starting indicator to show player levelling up

	player_ifs.close();

	// ----- Audio -----
	checkpoint[0].checkpointAudio = AEAudioLoadSound("Assets/AUDIO/Checkpoint.mp3");

	player.walkAudio = AEAudioLoadSound("Assets/AUDIO/Player walk.flac");
	player.levelUpAudio = AEAudioLoadSound("Assets/AUDIO/Player levelup.wav");

}

void player_init() {


	player.center.x			= player.initial_pos_x;		// Player's initial X position
	player.center.y			= player.initial_pos_y;		// Player's initial Y position
	player.Hp				= player.Max_Hp_Reset;		// Player's Health
	player.Max_Hp			= player.Max_Hp_Reset;		// Player's Maximum Health
	player.Lives			= player.Lives_Reset;		// Player's Lives
	player.Speed			= player.Speed_Reset;		// Player's Movement Speed
	player.Level			= player.Level_Reset;		// Player's Level
	player.XP				= player.XP_RESET;			// Player's XP
	//player.justLeveledUp	= FALSE;					// Indicator to show player levelling up
	player.status			= TRUE;

	player.Lives_dimensions.x = 50.f;
	player.Lives_dimensions.y = 50.f;

	//player.Lives_pos.x = 0;
	//player.Lives_y = 0;


	// -------- Checkpoint --------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		checkpoint[i].check = FALSE;					// Disable all checkpoints
	}

	// -------- Pause Menu --------
	isPaused = FALSE;		// Unpause game

}

void player_draw() {

	
	// ---------------- Player ----------------
	if (AEInputCheckCurr(AEVK_D)) {
		if ((num_of_Dpressed % 9) <= 4) AEGfxTextureSet(player.player_left1Tex, 0, 0);
		else if ((num_of_Dpressed % 9) >= 5) AEGfxTextureSet(player.player_left2Tex, 0, 0);
	}
	else if (AEInputCheckCurr(AEVK_A)) {
		if ((num_of_Apressed % 9) <= 4) AEGfxTextureSet(player.player_right1Tex, 0, 0);
		else if ((num_of_Apressed % 9) >= 5) AEGfxTextureSet(player.player_right2Tex, 0, 0);
	} 
	else AEGfxTextureSet(player.player_standTex, 0, 0);
	drawMesh(player.dimensions, player.center, PI);

	// -------------- Checkpoint --------------
	checkpoint_create(1000, 450, 0);// y+100
	checkpoint_create(3550, 950, 1);//6, 3350, 200, 11
	checkpoint_create(7100, 850, 2);//4, 7000, 750, 8
	checkpoint_create(2650, -150, 3);//(4, 2550, -250, 10);
	checkpoint_create(7000, 400, 4);//(7, 6900, 300, 14);

	// -------- Printing out no. of lives --------
	Render_Lives(1, 30.f);		// 1st life
	Render_Lives(2, 90.f);		// 2nd life
	Render_Lives(3, 150.f);		// 3rd life


	// -------- Drawing out HP bar ----------
	player.GameObjects::Render_HealthBar();

#ifdef DEBUG 
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		if (CollisionIntersection_RectRect(enemy1[i], player)) {
			std::cout << "Health percentage: " << health_percentage << std::endl;
		}
	}

	// -------- Printing out Hp ----------
	std::string hp_string = "Hp: ";
	AEGfxPrint(Albam_fontID, &hp_string[0], -0.05f, 0.85f, 1, 0.0f, 0.0f, 0.0f);
	std::string hp_counter_string = std::to_string(player.Hp);
	AEGfxPrint(Albam_fontID, &hp_counter_string[0], 0.15f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);
#endif // DEBUG

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

	// TEMP
	if (AEInputCheckCurr(AEVK_J)) {
		player.Max_Hp = 10;						// Player's Maximum Health
		player.Hp = player.Max_Hp;			// Player's Health
		std::cout << player.Max_Hp << std::endl;
	}
	// ---------  Player's movement   -----------
	// D key pressed
	if (AEInputCheckCurr(AEVK_D)) {
		player.center.x += FIXED_MOVEMENT * player.Speed;
		num_of_Dpressed++;
		//player.rotation -= 0.1f;
	}
	// A key pressed
	else if (AEInputCheckCurr(AEVK_A)) {
		player.center.x -= FIXED_MOVEMENT * player.Speed;
		num_of_Apressed++;
	}

	// --------  Player's level & XP   ----------
	// 40xp to level up for lvls 0-10 (1 enemy = 20xp)
	//if (player.XP == player.XP_TILL_10 && player.Level >= 0 && player.Level <= 10) {
	if (player.XP == 20 && player.Level >= 0 && player.Level <= 10) {
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
		respawn_player();
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




	// -------------  Update latest checkpoint for player  -------------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		if (player.center.x >= checkpoint[i].x1 && player.center.x <= checkpoint[i].x2 &&
			player.center.y >= checkpoint[i].y1 && player.center.y <= checkpoint[i].y2) {
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

void Render_Lives(s32 currLife, f32 offsetX) {
	// --- 1st Life ---
	if (player.Lives >= currLife)
		AEGfxTextureSet(player.fullLivesTex, 0, 0);
	else
		AEGfxTextureSet(player.emptyLivesTex, 0, 0);
	drawMesh(player.Lives_dimensions, AEVec2{ AEGfxGetWinMinX() + offsetX, AEGfxGetWinMaxY() - 25.0f }, PI);
}

void player_collision() {

	// left of screen
	if (player.center.x < -WINDOWLENGTH_X / 2.f + player.dimensions.x / 2.f)
		player.center.x = -WINDOWLENGTH_X / 2.f + player.dimensions.x / 2.f;

	// right of screen ---- CURRENTLY NO LIMIT ----

	// top of screen
	if (player.center.y > (WINDOWLENGTH_Y / 2.f - player.dimensions.y / 2.f) * 10)
		player.center.y = (WINDOWLENGTH_Y / 2.f - player.dimensions.y / 2.f) * 10;

	// bottom of screen
	if (player.center.y < -WINDOWLENGTH_Y / 2.f + player.dimensions.y / 2.f) {
		--player.Lives;
		player.Hp = player.Max_Hp;
		respawn_player();
	}
}

void respawn_player() {

	// ---------  Set player's position to latest checkpoint  ---------
	for (s32 i = NUM_OF_CHECKPOINT - 1; i >= 0; i--) {
		if (checkpoint[i].check) {
			player.center.x = checkpoint[i].x1 + 50;
			player.center.y = checkpoint[i].y1;
			break;
		}
		else {
			player.center.x = player.initial_pos_x;
			player.center.y = player.initial_pos_y;
		}
	}
	if (player.center.x > 0) {
		camera.x = player.center.x;
	}
	else {
		camera.x = 0;
	}
	camera.y = player.center.y;
}

void checkpoint_create(f32 x, f32 y, s32 index) {
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; ++i) {
		AEMtx33Scale(&checkpoint[i].scale, player.dimensions.x * 2, player.dimensions.y * 2);
		AEMtx33Rot(&checkpoint[i].rotate, PI);
		AEMtx33Trans(&checkpoint[i].translate, x, y);
		AEMtx33Concat(&checkpoint[i].transform, &checkpoint[i].rotate, &checkpoint[i].scale);
		AEMtx33Concat(&checkpoint[i].transform, &checkpoint[i].translate, &checkpoint[i].transform);
		AEGfxSetTransform(checkpoint[i].transform.m);
		AEGfxTextureSet(checkpoint[0].checkpointTex, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
