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

//#include <iostream>

Player_stats player;
Checkpoint checkpoint[NUM_OF_CHECKPOINT] = { {0, 1250, 1350, 250, 350}, {0, 2450, 2550, -50, 50} };

// ------  Text  ------
extern s8 Albam_fontID;
s8* lives_counter; // temp counter (Replacing with hearts?)
s8* level, * XP, * Hp;
// ----- Mesh & Texture -----
AEMtx33 scale, rotate, translate, transform;
AEGfxVertexList* pMesh;
AEGfxTexture* player_right1Tex, * player_right2Tex, * player_left1Tex, * player_left2Tex, * player_standTex, * checkpointTex;
int num_of_Apressed{ 0 }, num_of_Dpressed{ 0 };

// ----- Camera -----
f32 cameraX{}, cameraY{};

// ----- Enemy -----
//extern Enemy1_stats enemy1_a, enemy1_b;

void initialize_player() {

	player_standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_stand.png");
	player_left1Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk1_left.png");
	player_left2Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk2_left.png");
	player_right1Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk1_right.png");
	player_right2Tex = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_walk2_right.png");
	checkpointTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/cactus.png");
	pMesh = create_Square_Mesh();

	bullet_initialise();
}

void draw_player() {

	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEGfxSetTransparency(1.0f);
	//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	// ---------------- Player ----------------
	// Creates a player size 50x50
	AEMtx33Scale(&scale, PLAYER_WIDTH, PLAYER_HEIGHT);
	// Rotate player
	AEMtx33Rot(&rotate, PI);
	// Move player when A / D keys pressed
	AEMtx33Trans(&translate, player.x, player.y);
	// Concat the matrices (TRS)
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to use
	AEGfxSetTransform(transform.m);
	if (AEInputCheckCurr(AEVK_D)) {
		if ((num_of_Dpressed % 9) <= 4) AEGfxTextureSet(player_left1Tex, 0, 0);
		else if ((num_of_Dpressed % 9) >= 5) AEGfxTextureSet(player_left2Tex, 0, 0);
	}
	else if (AEInputCheckCurr(AEVK_A)) {
		if ((num_of_Apressed % 9) <= 4) AEGfxTextureSet(player_right1Tex, 0, 0);
		else if ((num_of_Apressed % 9) >= 5) AEGfxTextureSet(player_right2Tex, 0, 0);
	} 
	else AEGfxTextureSet(player_standTex, 0, 0);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// -------------- Checkpoint --------------
	checkpoint_create(1300, 300);
	checkpoint_create(2500, 0);

	// -------- Printing out no. of lives --------
	if (player.Lives == 3) lives_counter = (s8*)"Lives: 3";
	else if (player.Lives == 2) lives_counter = (s8*)"Lives: 2";
	else if (player.Lives == 1) lives_counter = (s8*)"Lives: 1";
	else if (player.Lives == 0) lives_counter = (s8*)"YOU ARE DEAD!";

	// -------- Printing out Hp ----------
	if (player.Hp == 5 && player.Lives > 0) Hp = (s8*)"HP: 5";
	else if (player.Hp == 4 && player.Lives > 0) Hp = (s8*)"HP: 4";
	else if (player.Hp == 3 && player.Lives > 0) Hp = (s8*)"HP: 3";
	else if (player.Hp == 2 && player.Lives > 0) Hp = (s8*)"HP: 2";
	else if (player.Hp == 1 && player.Lives > 0) Hp = (s8*)"HP: 1";
	else if (player.Lives == 0 || player.Hp == 0) Hp = (s8*)"HP: 0";


	s8* notif = nullptr;

	if (player.Level == 0) level = (s8*)"Level: 0";
	else if (player.Level == 1) level = (s8*)"Level: 1";

	

	if (player.XP == 0) XP = (s8*)"XP: 0";
	else if (player.XP == 10) XP = (s8*)"XP: 10";

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(Albam_fontID, lives_counter, -1.0f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, level, -1.0f, 0.7f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, XP, -1.0f, 0.55f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, Hp, -0.15f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);
	


	// --------- Portal draw & update ---------
	draw_portal();
	AEVec2Set(&player.center, player.x, player.y);

	// ---------  Firing of bullets   -----------
	/*if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(player.x, player.y, 1);
	}
	else {
		weapon_fire(player.x, player.y, 0);
	}*/
}

void update_player() {

	// --------  Setting player's position into a vector --------
	//AEVec2Set(&player.center, player.x, player.y);

	// ---------  Player's movement   -----------
	// D key pressed
	if (AEInputCheckCurr(AEVK_D)) {
		player.x += 5;
		num_of_Dpressed++;
		//player.rotation -= 0.1f;
	}
	// A key pressed
	else if (AEInputCheckCurr(AEVK_A)) {
		player.x -= 5;
		num_of_Apressed++;
		//player.rotation += 0.1f;
	}

	// --------  Player's level & XP   ----------
	// FOR NOW ONLY: 20xp to level up (1 enemy = 10xp)
	if (player.XP == 20) {
		player.Level++;
		player.XP -= 20;
		player.justLeveledUp = TRUE;
	}

	// --------    Player Hp & Lives   -----------
	if (player.Hp <= 0) {
		player.Lives--;
		player.Hp += 5;
	}

	// ---------  Firing of bullets   -----------
	/*if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(player.x, player.y, 1);
	}
	else {
		weapon_fire(player.x, player.y, 0);
	}*/

	// ---------  Portal creation   -----------
	//draw_portal(player.x, player.y);
	//AEVec2Set(&player.center, player.x, player.y);


	// ------------  Collision   --------------
	player_collision();
	//enemy_collision(&player); 
	enemy3_collision(&player);
	//enemy_collision(&player, enemy1_a);
	//enemy_collision(&player, enemy1_b);

	// -------------  Camera   ---------------
	AEGfxSetCamPosition(cameraX, cameraY);
	if (player.x > 0)
		cameraX = player.x;

	if (player.x <= 0)
		cameraX = 0;

	if (player.y > 0)
		cameraY = player.y;

	if (player.y <= 0)
		cameraY = 0;
	
	if (AEInputCheckCurr(AEVK_W))
		cameraY += 2.0f;

	if (AEInputCheckCurr(AEVK_S))
		cameraY -= 2.0f;

	// -------------  Update latest checkpoint for player  -------------
	for (s32 i = 0; i < NUM_OF_CHECKPOINT; i++) {
		if (player.x >= checkpoint[i].x1 && player.x <= checkpoint[i].x2 &&
			player.y >= checkpoint[i].y1 && player.y <= checkpoint[i].y2) {
			checkpoint[i].check = TRUE;
			//checkpoint[i-1].check = 0;    //-----> If player position updates according to most recent checkpoint & NOT furthest checkpoint
		}
	}
}

void unload_player() {
	
}

void player_collision() {


	// left of screen
	if (player.x < -WINDOWLENGTH_X / 2 + PLAYER_WIDTH / 2)
		player.x = -WINDOWLENGTH_X / 2 + PLAYER_WIDTH / 2;

	// right of screen ---- CURRENTLY NO LIMIT ----
	//if (player.x > WINDOWXLENGTH / 2 - PLAYER_WIDTH / 2)
	//	player.x = WINDOWXLENGTH / 2 - PLAYER_WIDTH / 2;

	// top of screen
	//if (player.y > WINDOWLENGTH_Y / 2 - PLAYER_HEIGHT / 2)
	//	player.y = WINDOWLENGTH_Y / 2 - PLAYER_HEIGHT / 2;

	// bottom of screen
	if (player.y < -WINDOWLENGTH_Y / 2 + PLAYER_HEIGHT / 2) {
		--player.Lives;

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

void checkpoint_create(f32 x, f32 y) {

	AEMtx33Scale(&scale, PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Trans(&translate, x, y);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(checkpointTex, 0, 0);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}
