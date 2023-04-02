/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Enemy3.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file implements the functions for Enemy3.
==================================================================================*/
#include "AEEngine.h"
#include <AEGraphics.h>
#include "Player.hpp"
#include "Utilities.hpp"
#include "Enemy3.hpp"


// ----- Mesh & Texture -----
AEGfxTexture* enemy3;
AEGfxTexture* enemy3_warning;
extern AEGfxVertexList* square_mesh;	// Created square mesh

extern AEMtx33 scale, rotate, translate, transform; // TRS

// ----- Enemy -----
Enemy3_stats enemy3_a; 
bool enemy3_a_Dead, damage_allowed3{ TRUE }, going_left{ FALSE }; 
const float offscreen_offset{ 1800.f };	// value offscreen where enemy3 matches player's y-value
const float offscreen_spawn{ 3000.f };  // value offscreen where enemy3 spawns at start of game

// ----- Player -----
extern Player_stats player; // for player position, HP, XP

// ----- Pause Menu -----
extern bool isPaused;		// movement while game is !isPaused

// ----- Audio -----
extern AEAudio zoomAudio, playerDamageAudio;
extern AEAudioGroup soundGroup;

void enemy3_load() {
	// enemy3
	enemy3 = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0053.png");

	// enemy 3 warning label
	enemy3_warning = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/powerup_wings.png");
}

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/

void enemy3_init() {

	// spawn 3000.f outside left edge of screen
	enemy3_a.x = -WINDOWLENGTH_X/2.f - offscreen_spawn;
	// initialize y to 0
	enemy3_a.y = 0;
	
}

void draw_enemy3() {
	//draws enemy3 if alive
	if (enemy3_a.Hp > 0 && enemy3_a_Dead == FALSE) {

		AEGfxTextureSet(enemy3, 0, 0);
		drawMesh(AEVec2{ ENEMY3_WIDTH, ENEMY3_HEIGHT }, enemy3_a.center, PI);

		// set center vector
		AEVec2Set(&enemy3_a.center, enemy3_a.x, enemy3_a.y);

		// right edge of screen < enemy3 x position < right edge of screen + offscreen_offset & enemy going left
		if (enemy3_a.x > AEGfxGetWinMaxX() && enemy3_a.x < AEGfxGetWinMaxX() + offscreen_offset
			&& going_left) {
			AEGfxTextureSet(enemy3_warning, 0, 0);
			drawMesh(AEVec2{ WARNING_WIDTH, WARNING_HEIGHT }, AEVec2{ AEGfxGetWinMaxX() - WARNING_WIDTH/2.f, enemy3_a.y }, PI);
		}
		// left edge of screen - offscreen_offset < enemy 3 x position < left edge of screen & enemy going right
		else if (enemy3_a.x < AEGfxGetWinMinX() && enemy3_a.x > AEGfxGetWinMinX() - offscreen_offset
			&& !going_left) {
			AEGfxTextureSet(enemy3_warning, 0, 0);
			drawMesh(AEVec2{ WARNING_WIDTH, WARNING_HEIGHT }, AEVec2{ AEGfxGetWinMinX() + WARNING_WIDTH/2.f, enemy3_a.y }, PI);
		}

		
	}
	// ------- XP for player -------
	else if (enemy3_a.Hp <= 0 && enemy3_a_Dead == FALSE) {
		player.XP += 30;
		enemy3_a_Dead = TRUE;
	}
}

void enemy3_update(Player_stats* player) {
	Enemy3_stats* Enemy3_a = &enemy3_a;
	if (!isPaused) {
		s32 value = AEFrameRateControllerGetFrameCount() % 7500;
		
		// moves right
		if (value <= 3750) {
			//enemy3 y position is determined (matches player y pos) when it is 1800.f(offscreen_offset)+ away from left/right edge of screen

			if (enemy3_a.x < AEGfxGetWinMinX() - offscreen_offset)
				Enemy3_a->y = player->center.y;
			
			Enemy3_a->x += 4.f;
			going_left = false;
		}

		// moves left
		else {
			//enemy3 y position is determined (matches player y pos) when it is 1800.f(offscreen_offset)+ away from left/right edge of screen

			if (enemy3_a.x > AEGfxGetWinMaxX() + offscreen_offset)
				Enemy3_a->y = player->center.y;

			Enemy3_a->x -= 4.f;
			going_left = true;
		}

	}

}
void enemy3_free() {

}

void enemy3_unload() {
	// Enemy3 texture unload
	AEGfxTextureUnload(enemy3);
	// Warning label unload
	AEGfxTextureUnload(enemy3_warning);
}

void enemy3_collision() {
	AEVec2 player_vec{ player.center.x , player.center.y };

		if (damage_allowed3) {
			// decreases 1 player hp whenever player and enemy3 collide
			if (AETestRectToRect(&enemy3_a.center, ENEMY3_WIDTH/10, ENEMY3_HEIGHT/10, &player_vec, player.dimensions.x/10, player.dimensions.y/10)) {
				if (player.isShieldActive) {
					player.isShieldActive = FALSE;
				}
				else {
					--player.Hp;
				}
				// disables damage temporarily once collided
				damage_allowed3 = FALSE;

				// Player's taking damage sfx
				AEAudioPlay(playerDamageAudio, soundGroup, 0.25f, 1.f, 0);
			}
		}

		else {
			// allows damage again 100 frames after player-enemy3 collision 
			if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
				damage_allowed3 = TRUE;
			}
		}
	
}
