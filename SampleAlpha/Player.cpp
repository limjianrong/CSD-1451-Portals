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

// for fontID
#include "GameState_Mainmenu.hpp"

//#include <iostream>

Player_stats player;
Checkpoint checkpoint[NUM_OF_CHECKPOINT] = { {0, 250, 350, 150, 250}, {0, 700, 800, 50, 150} };

// ------  Text  ------
extern s8 fontID;
s8* lives_counter;

// ----- Mesh & Texture -----
AEMtx33 scale, rotate, translate, transform;
AEGfxVertexList* pMesh;
AEGfxTexture *playerTex, *checkpointTex;

// ----- Camera -----
f32 cameraX{}, cameraY{};

void initialize_player() { 

	playerTex = AEGfxTextureLoad("Assets/simplified_png/PNG/Tiles/platformPack_tile024.png");
	checkpointTex = AEGfxTextureLoad("Assets/fullpack/PNG/Items/flagBlue2.png");
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
	AEMtx33Rot(&rotate, player.rotation);
	// Move player when A / D keys pressed
	AEMtx33Trans(&translate, player.x, player.y);
	// Concat the matrices (TRS)
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to use
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(playerTex, 0, 0);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// -------------- Checkpoint --------------
	checkpoint_create(300, 200);
	checkpoint_create(750, 100);

	// -------- Printing out no. of lives --------
	if (player.Lives == 3) lives_counter = (s8*)"Lives: 3";
	else if (player.Lives == 2) lives_counter = (s8*)"Lives: 2";
	else if (player.Lives == 1) lives_counter = (s8*)"Lives: 1";
	else if (player.Lives == 0) lives_counter = (s8*)"YOU ARE DEAD!";

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(fontID, lives_counter, -1.0f, 0.85f, 1.0f, 0.0f, 0.0f, 0.0f);

}

void update_player() {

	// ---------  Player's movement   -----------
	// A key pressed
	if (AEInputCheckCurr(AEVK_A)) {
		player.x -= 5;
		player.rotation += 0.1f;
	}
	// D key pressed
	else if (AEInputCheckCurr(AEVK_D)) {
		player.x += 5;
		player.rotation -= 0.1f;
	}

	// ---------  Firing of bullets   -----------
	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(player.x, player.y, 1);
	}
	else {
		weapon_fire(player.x, player.y, 0);
	}

	// ---------  Portal creation   -----------
	draw_portal(player.x, player.y);


	// ------------  Collision   --------------
	player_collision();

	// -------------  Camera   ---------------
	AEGfxSetCamPosition(cameraX, cameraY);
	if (player.x > 0) {
		cameraX = player.x;
	}
	if (AEInputCheckCurr(AEVK_W)) {
		cameraY += 2.0f;
	}
	if (AEInputCheckCurr(AEVK_S)) {
		cameraY -= 2.0f;
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

void unload_player() {
	//AEGfxDestroyFont(livesID);
}

void player_collision() {


	// left of screen
	if (player.x < -WINDOWXLENGTH / 2 + PLAYER_WIDTH / 2)
		player.x = -WINDOWXLENGTH / 2 + PLAYER_WIDTH / 2;

	// right of screen ---- CURRENTLY NO LIMIT ----
	//if (player.x > WINDOWXLENGTH / 2 - PLAYER_WIDTH / 2)
	//	player.x = WINDOWXLENGTH / 2 - PLAYER_WIDTH / 2;

	// top of screen
	if (player.y > WINDOWYLENGTH / 2 - PLAYER_HEIGHT / 2)
		player.y = WINDOWYLENGTH / 2 - PLAYER_HEIGHT / 2;

	// bottom of screen
	if (player.y < -WINDOWYLENGTH / 2 + PLAYER_HEIGHT / 2) {
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
