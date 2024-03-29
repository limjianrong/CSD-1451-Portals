/*==================================================================================
* All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Player.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This source file contains declaration of functions for player related logic
  Contains declaration of functions that updates player's level based
  on XP, player's movement, player's collision and checkpoints within the game
==================================================================================*/

#include "AEEngine.h"

#include "Player.hpp"
#include "PortalFeature.hpp"
#include "Utilities.hpp"
#include "DrawLevel.hpp"
#include "Enemy.hpp"
#include "Enemy3.hpp"
#include "Camera.hpp"
#include "GameState_Mainmenu.hpp"

// for gameStates
#include "GameStateList.hpp"
#include "GameStateManager.hpp"

// for isPaused
#include "GameState_Platformer.hpp"

#include <string>
#include <fstream>
#include <iostream>

Player_stats player;
Checkpoint checkpoint[NUM_OF_CHECKPOINT];

// ------  Text  ------
extern s8 Albam_fontID;
s8* level, * XP, * Hp;
// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh

// --- Audio ---
extern AEAudio checkpointAudio, walkAudio, playerDeathAudio;
extern AEAudioGroup soundGroup, musicGroup;
static bool isPlayingAudio = FALSE;

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
	player.player_standTex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_idle.png");
	player.player_left1Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk3.png");
	player.player_left2Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk4.png");
	player.player_right1Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk1.png");
	player.player_right2Tex = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	checkpoint[0].checkpointTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/other/flagGreen_up.png");

	player.fullLivesTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/redCrystal.png");
	player.emptyLivesTex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/outlineCrystal.png");
	player_ifs.open("Assets/textFiles/player_stats.txt");
#ifdef debug
	if (!player_ifs) {
		std::cout << "\nFailed to open player_stats.txt";
	}
#endif
	// -------- Player --------
	std::string str{};
	player_ifs >> str >> player.dimensions.x;		// Player's width
	player_ifs >> str >> player.dimensions.y;		// Player's height
	player_ifs >> str >> player.initial_pos_x;		// Player's initial X position
	player_ifs >> str >> player.initial_pos_y;		// Player's initial Y position
	player_ifs >> str >> player.highest_level;		// Level cap of 30 lvls

	player_ifs >> str >> player.XP_TILL_SYSTEM1;	// Currently requires 40 XP to level up for lvls 0-5
	player_ifs >> str >> player.XP_TILL_SYSTEM2;	// Currently requires 60 XP to level up for lvls 5-10
	player_ifs >> str >> player.XP_TILL_SYSTEM3;	// Currently requires 80 XP to level up for lvls 10-15
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

}

void player_init() {


	player.center.x			= static_cast<f32>(player.initial_pos_x);		// Player's initial X position
	player.center.y			= static_cast<f32>(player.initial_pos_y);		// Player's initial Y position
	player.Hp				= player.Max_Hp_Reset;							// Player's Health
	player.Max_Hp			= player.Max_Hp_Reset;							// Player's Maximum Health
	player.Lives			= player.Lives_Reset;							// Player's Lives
	player.Speed			= player.Speed_Reset;							// Player's Movement Speed
	player.Level			= player.Level_Reset;							// Player's Level
	player.XP				= player.XP_RESET;								// Player's XP
	//player.justLeveledUp	= FALSE;										// Indicator to show player levelling up
	player.status			= TRUE;											// Player's status
	player.requiredXP		= 40;											// Required XP to levelup

	player.Lives_dimensions.x = 50.f;
	player.Lives_dimensions.y = 50.f;


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
		if ((num_of_Dpressed % max_Dpresses) <= range_Dpresses) AEGfxTextureSet(player.player_right1Tex, 0, 0);
		else AEGfxTextureSet(player.player_right2Tex, 0, 0);
	}
	else if (AEInputCheckCurr(AEVK_A)) {
		if ((num_of_Apressed % max_Dpresses) <= range_Dpresses) AEGfxTextureSet(player.player_left1Tex, 0, 0);
		else AEGfxTextureSet(player.player_left2Tex, 0, 0);
	}
	else AEGfxTextureSet(player.player_standTex, 0, 0);
	drawMesh(player.dimensions, player.center, NULL);

	// -------------- Checkpoint --------------
	checkpoint_create(1000, 450, 0);
	checkpoint_create(3550, 950, 1);
	checkpoint_create(2650, -150, 2);
	checkpoint_create(7100, 850, 3);
	checkpoint_create(7000, 400, 4);

	// -------- Drawing Lives UI --------
	AEGfxPrint(Albam_fontID, (s8*)"Lives:", -0.95f, 0.85f, 1, 0.f, 0.f, 0.f);
	Render_Lives(1, 220.f);		// 1st life
	Render_Lives(2, 280.f);		// 2nd life
	Render_Lives(3, 340.f);		// 3rd life
	Render_Lives(4, 400.f);		// 4th life
	Render_Lives(5, 460.f);		// 5th life


	// -------- Drawing out HP bar ----------
	player.GameObjects::Render_HealthBar();

	// -------- Drawing out Level & XP bar ----------
	Render_LevelnXP();

	// --- Level text ---
	AEGfxPrint(Albam_fontID, (s8*)"Level", 0.8f, -0.98f, 1.0f, 0.f, 0.f, 0.f);
	std::string level_counter_string = std::to_string(player.Level);
	AEGfxPrint(Albam_fontID, &level_counter_string[0], 0.95f, -0.98f, 1.0f, 0.f, 0.f, 0.f);

	// --- XP text ---
	AEGfxPrint(Albam_fontID, (s8*)"Xp", -0.99f, -0.98f, 1, 0.f, 0.f, 0.f);

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

	// -------- Printing out Level ----------
	std::string level_string = "Level: ";
	AEGfxPrint(Albam_fontID, &level_string[0], -1.0f, 0.70f, 1, 0.0f, 0.0f, 0.0f);
	std::string level_counter_string = std::to_string(player.Level);
	AEGfxPrint(Albam_fontID, &level_counter_string[0], -0.7f, 0.70f, 1.0f, 0.0f, 0.0f, 0.0f);

	// -------- Printing out Xp ----------
	std::string xp_string = "Xp: ";
	AEGfxPrint(Albam_fontID, &xp_string[0], -1.0f, 0.55f, 1, 0.0f, 0.0f, 0.0f);
	std::string xp_counter_string = std::to_string(player.XP);
	AEGfxPrint(Albam_fontID, &xp_counter_string[0], -0.7f, 0.55f, 1.0f, 0.0f, 0.0f, 0.0f);
#endif // DEBUG

}

void player_update() {

	// TEMP
	if (AEInputCheckCurr(AEVK_J)) {
		player.Max_Hp = 10;						// Player's Maximum Health
		player.Hp = player.Max_Hp;				// Player's Health
	}
	// ---------  Player's movement   -----------
	// D key pressed
	if (AEInputCheckCurr(AEVK_D)) {

		player.center.x += FIXED_MOVEMENT * player.Speed;
		num_of_Dpressed++;
		
		// Player movement audio
		if ((num_of_Dpressed % max_AudioDelay) <= 0) AEAudioPlay(walkAudio, soundGroup, 0.5f, 1.f, 0);
	}
	// A key pressed
	else if (AEInputCheckCurr(AEVK_A)) {

		player.center.x -= FIXED_MOVEMENT * player.Speed;
		num_of_Apressed++;

		// Player movement audio
		if ((num_of_Apressed % max_AudioDelay) <= 0) AEAudioPlay(walkAudio, soundGroup, 0.5f, 1.f, 0);
	}

	// --------  Player's level & XP   ----------
	level_xp_system_update(player.XP_TILL_SYSTEM1, LEVEL_SYSTEM_0, LEVEL_SYSTEM_1);		// Currently 40xp for lvls 0-5
	level_xp_system_update(player.XP_TILL_SYSTEM2, LEVEL_SYSTEM_1, LEVEL_SYSTEM_2);		// Currently 60xp for lvls 5-10
	level_xp_system_update(player.XP_TILL_SYSTEM3, LEVEL_SYSTEM_2, LEVEL_SYSTEM_3);		// Currently 80xp for lvls 10-15

	// --------    Player Hp & Lives   -----------
	if (player.Hp <= 0) {
		player.Lives--;
		player.Hp = player.Max_Hp;
		respawn_player();
		reset_portals();

		// Player's death sfx
		AEAudioPlay(playerDeathAudio, soundGroup, 0.75f, 1.f, 0);
	}
	if (player.Lives <= 0) {
		// Stop all BGM
		AEAudioStopGroup(musicGroup);

		gGameStateNext = GS_Lose;
	}


	// ------------  Collision   --------------
	player_collision();
	enemy3_collision();


	// -------------  Update latest checkpoint for player  -------------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		if (player.center.x >= checkpoint[i].x1 && player.center.x <= checkpoint[i].x2 &&
			player.center.y >= checkpoint[i].y1 && player.center.y <= checkpoint[i].y2) {

			if (checkpoint[i].check == FALSE) {
				checkpoint[i].check = TRUE;
				//checkpoint[i-1].check = 0;    //-----> If player position updates according to most recent checkpoint & NOT furthest checkpoint
				
				// Checkpoint audio
				AEAudioPlay(checkpointAudio, soundGroup, 0.75f, 1.f, 0);
			}
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

void level_xp_system_update(s32 requiredXP, s32 levelSystem_LowerRange, s32 levelSystem_HigherRange) {

	if (player.XP >= requiredXP && player.Level >= levelSystem_LowerRange && player.Level <= levelSystem_HigherRange) {
		player.requiredXP = requiredXP;	// Set requiredXP
		player.Level++;					// Levels up player
		player.XP -= requiredXP;		// Deduct requiredXP to levelup from player
		player.justLeveledUp = TRUE;	// Enable flag
	}
}

void Render_Lives(s32 currLife, f32 offsetX) {
	// --- 1st Life ---
	if (player.Lives >= currLife)
		AEGfxTextureSet(player.fullLivesTex, 0, 0);
	else
		AEGfxTextureSet(player.emptyLivesTex, 0, 0);
	drawMesh(player.Lives_dimensions, AEVec2{ AEGfxGetWinMinX() + offsetX, AEGfxGetWinMaxY() - Lives_OffsetY }, NULL);
}

void Render_LevelnXP() {
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTintColor(255, 255, 0, 1.f);
	drawMesh(AEVec2{ WINDOWLENGTH_X - XP_BAR_OFFSETX, XP_BAR_HEIGHT }, AEVec2{ origin.x - XP_BAR_OFFSETX / 2.f, origin.y - WINDOWLENGTH_Y / 2.f + XP_BAR_HEIGHT / 2.f }, NULL);

	AEGfxSetTintColor(0, 255, 255, 1.f);
	drawMesh(AEVec2{ (float)player.XP / (float)player.requiredXP * (WINDOWLENGTH_X - 150.f) , XP_BAR_HEIGHT },
			 AEVec2{ (float)origin.x - XP_BAR_OFFSETX / 2.f - (((float)player.requiredXP - (float)player.XP) / (float)player.requiredXP * (WINDOWLENGTH_X - XP_BAR_OFFSETX) / 2.f), origin.y - WINDOWLENGTH_Y / 2.f + XP_BAR_HEIGHT / 2.f }, NULL);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
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

		// Player death audio
		AEAudioPlay(playerDeathAudio, soundGroup, 0.5f, 1.f, 0);
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
			player.center.x = static_cast<f32>(player.initial_pos_x);
			player.center.y = static_cast<f32>(player.initial_pos_y);
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

	AEGfxTextureSet(checkpoint[0].checkpointTex, 0, 0);
	drawMesh(AEVec2{ player.dimensions.x * 2, player.dimensions.y * 2 }, AEVec2{ x, y }, NULL);

	checkpoint[index].x1 = x - CHECKPOINT_WIDTH;
	checkpoint[index].x2 = x + CHECKPOINT_WIDTH;
	checkpoint[index].y1 = y - CHECKPOINT_HEIGHT;
	checkpoint[index].y2 = y + CHECKPOINT_HEIGHT;
}
