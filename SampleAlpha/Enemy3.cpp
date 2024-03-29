/*==================================================================================
* All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
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

// ----- Enemy -----
Enemy3_stats enemy3_a; 
bool damage_allowed3{ TRUE }, going_left{ FALSE }; 
const float offscreen_offset{ 1800.f };	// value offscreen where enemy3 matches player's y-value
const float offscreen_spawn{ 3000.f };  // value offscreen where enemy3 spawns at start of game

// ----- Player -----
extern Player_stats player;		// for player position, HP, XP
const int XP_increase{ 30 };	// player gains XP for killing enemy3

// ----- Pause Menu -----
extern bool isPaused;			// movement while game is !isPaused

// ----- Audio -----
extern AEAudio zoomAudio, playerDamageAudio, deathAudio;
extern AEAudioGroup soundGroup;

/*!****************************************************************************************************
\brief
	Loads texture for enemy
*******************************************************************************************************/
void enemy3_load() {
	// enemy3
	enemy3 = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0053.png");

	// enemy 3 warning label
	enemy3_warning = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/powerup_wings.png");
}

/*!****************************************************************************************************
\brief
	Initializes spawn position for enemy
*******************************************************************************************************/
void enemy3_init() {

	enemy3_a.dimensions.x = ENEMY3_WIDTH;		// Enemy3's Width
	enemy3_a.dimensions.y = ENEMY3_HEIGHT;		// Enemy3's Height
	enemy3_a.Hp = 5;							// Enemy3's Health
	enemy3_a.Max_Hp = 5;						// Enemy3's Max Health
	enemy3_a.status = TRUE;						// TRUE for alive, FALSE for dead
	enemy3_a.center.x = -WINDOWLENGTH_X / 2.f - offscreen_spawn; // spawn 3000.f outside left edge of screen
	enemy3_a.center.y = 0;										// initialize y to 0
	
	
}

/*!****************************************************************************************************
\brief
	Draws all enemy3 related texture
*******************************************************************************************************/
void draw_enemy3() {
	//draws enemy3 if alive
	if (enemy3_a.Hp > 0 && enemy3_a.status == TRUE) {

		AEGfxTextureSet(enemy3, 0, 0);
		drawMesh(AEVec2{ ENEMY3_WIDTH, ENEMY3_HEIGHT }, enemy3_a.center, NULL);

		// ----- Draw enemy HP bar -----
		enemy3_a.GameObjects::Render_HealthBar();

		// set center vector
		//AEVec2Set(&enemy3_a.center, enemy3_a.x, enemy3_a.y);

		// right edge of screen < enemy3 x position < right edge of screen + offscreen_offset & enemy going left
		if (enemy3_a.center.x > AEGfxGetWinMaxX() && enemy3_a.center.x < AEGfxGetWinMaxX() + offscreen_offset
			&& going_left) {
			AEGfxTextureSet(enemy3_warning, 0, 0);
			drawMesh(AEVec2{ WARNING_WIDTH, WARNING_HEIGHT }, AEVec2{ AEGfxGetWinMaxX() - WARNING_WIDTH/2.f, enemy3_a.center.y }, NULL);
		}
		// left edge of screen - offscreen_offset < enemy 3 x position < left edge of screen & enemy going right
		else if (enemy3_a.center.x < AEGfxGetWinMinX() && enemy3_a.center.x > AEGfxGetWinMinX() - offscreen_offset
			&& !going_left) {
			AEGfxTextureSet(enemy3_warning, 0, 0);
			drawMesh(AEVec2{ WARNING_WIDTH, WARNING_HEIGHT }, AEVec2{ AEGfxGetWinMinX() + WARNING_WIDTH/2.f, enemy3_a.center.y }, NULL);
		}

		
	}
}

/*!****************************************************************************************************
\brief
	Update enemy3 data

\param player
	Pointer to player data
*******************************************************************************************************/
void enemy3_update(Player_stats* _player) {
	Enemy3_stats* Enemy3_a = &enemy3_a;
	if (!isPaused) {
		s32 value = AEFrameRateControllerGetFrameCount() % 7500;
		
		// moves right
		if (value <= 3750) {
			//enemy3 y position is determined (matches player y pos) when it is 1800.f(offscreen_offset)+ away from left/right edge of screen

			if (enemy3_a.center.x < AEGfxGetWinMinX() - offscreen_offset)
				Enemy3_a->center.y = _player->center.y;
			
			Enemy3_a->center.x += 4.f;
			going_left = false;
		}

		// moves left
		else {
			//enemy3 y position is determined (matches player y pos) when it is 1800.f(offscreen_offset)+ away from left/right edge of screen

			if (enemy3_a.center.x > AEGfxGetWinMaxX() + offscreen_offset)
				Enemy3_a->center.y = _player->center.y;

			Enemy3_a->center.x -= 4.f;
			going_left = true;
		}


		// ------- XP for player -------
		if (enemy3_a.Hp <= 0 && enemy3_a.status == TRUE) {
			_player->XP += static_cast<s32>(ENEMY3_DROPPED_XP);
			enemy3_a.status = FALSE;

			// Audio upon death
			AEAudioPlay(deathAudio, soundGroup, 1.f, 1.f, 0);
		}
	}

}

/*!****************************************************************************************************
\brief
	Unloads all enemy3 related textures
*******************************************************************************************************/
void enemy3_unload() {
	// Enemy3 texture unload
	AEGfxTextureUnload(enemy3);
	// Warning label unload
	AEGfxTextureUnload(enemy3_warning);
}

/*!****************************************************************************************************
\brief
	Collision detection between player and enemy3
*******************************************************************************************************/
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
