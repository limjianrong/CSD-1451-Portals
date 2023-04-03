/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Enemy.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This source file contains all the declaraction for functions and derived struct class used for enemy1 and enemy2
  This includes drawing of their mesh with respective texture, movement, collision and related logic
==================================================================================*/

#include "AEEngine.h"
#include <AEGraphics.h>
#include <ctime>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Enemy3.hpp" 
#include "Utilities.hpp"
#include "Boss.hpp"
#include "GameState_Platformer.hpp"

#include <iostream>
// ----- Mesh & Texture -----
extern AEGfxVertexList* square_mesh;	// Created square mesh


// ----- Objects -----
extern Player_stats player;
Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2
extern Enemy3_stats enemy3_a;			// Enemy3
Bullet bullet_enemy2[MAX_ENEMIES_2];	// Array of struct enemy2's bullet

// ----- Pause Menu -----
extern bool isPaused;

// ---- FileIO ----
std::ifstream enemies_ifs;

// ----- Audio -----
extern AEAudio damageAudio, deathAudio, bulletAudio, playerDamageAudio;
extern AEAudioGroup soundGroup;


void enemies_load() {

	// ---- Enemy1 Texture ----
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		enemy1[i].texture = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0055.png");
	}

	// ---- Enemy2 Texture ----
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		enemy2[i].enemy2_fly1 = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0051.png");
		enemy2[i].enemy2_fly2 = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0051.png");
		enemy2[i].enemy2_dead = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0051.png");
	}
	enemy2[0].bullet = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Items/blueGem.png");
}

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/
void enemies_init() {

	enemies_ifs.open("Assets/textFiles/enemies.txt");
	if (!enemies_ifs) {
		std::cout << "\nFailed to open enemies.txt";
	}
	std::string str{};

	// Enemy1
	enemies_ifs >> str >> ENEMY1_WIDTH;
	enemies_ifs >> str >> ENEMY1_HEIGHT;
	enemies_ifs >> str >> ENEMY1_DROPPED_XP;
	enemies_ifs >> str >> MAX_FRAME_MOVEMENT;
	enemies_ifs >> str >> CHANGE_FRAME_MOVEMENT;
	enemies_ifs >> str >> ENEMY1_MOVEMENTX;
	enemies_ifs >> str >> HP_RESET_1;
	enemies_ifs >> str >> MAX_HP_RESET_1;
	enemies_ifs >> str >> MOVEMENTCOUNTER_RESET;

	// Enemy2
	enemies_ifs >> str >> ENEMY2_WIDTH;
	enemies_ifs >> str >> ENEMY2_HEIGHT;
	enemies_ifs >> str >> ENEMY2_TIMER;
	enemies_ifs >> str >> ENEMY2_DROPPED_XP;
	enemies_ifs >> str >> HP_RESET_2;
	enemies_ifs >> str >> MAX_HP_RESET_2;
	enemies_ifs >> str >> MAX_MOVEMENT;
	enemies_ifs >> str >> CHANGE_MOVEMENT;
	enemies_ifs >> str >> ENEMY2_MOVEMENTY;
	enemies_ifs >> str >> RangeToEnableBulletDelay;
	enemies_ifs >> str >> Bullet_Displacement_PerFrame;
	enemies_ifs >> str >> Range_x;
	enemies_ifs >> str >> Range_y;

	//Bullet
	enemies_ifs >> str >> BULLET_WIDTH;
	enemies_ifs >> str >> BULLET_HEIGHT;
	enemies_ifs >> str >> BULLET_SPEED;

	enemies_ifs.close();

	// ------- Enemy 1 -------
	enemy1_create(600, 90, 0);
	enemy1_create(2425, 590, 1);
	enemy1_create(6750, 1090, 2);


	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {

		//enemy1[i].rotation = PI;								// Enemy1's Rotation
		enemy1[i].dimensions.x		= ENEMY1_WIDTH;				// Enemy1's Width
		enemy1[i].dimensions.y		= ENEMY1_HEIGHT;			// Enemy1's Height
		enemy1[i].Hp				= HP_RESET_1;				// Enemy1's Health
		enemy1[i].Max_Hp			= MAX_HP_RESET_1;			// Enemy1's Max Health
		enemy1[i].movementCounter	= MOVEMENTCOUNTER_RESET;	// Enemy1's Movement Counter
		enemy1[i].status			= TRUE;						// TRUE for alive, FALSE for dead
		enemy1[i].isDamageAllowed	= TRUE;						// Indicator for damage delay of player & enemy1

	}


	// ------- Enemy 2 & bullets -------
	enemy2_create(3350, 900, 0);
	enemy2_create(3800, 875, 1);
	enemy2_create(3800, 925, 2);
	enemy2_create(3700, 250, 3);
	enemy2_create(3700, 300, 4);


	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {

		// ---- Enemy2 ----
		enemy2[i].dimensions.x			= ENEMY2_WIDTH;					// Enemy2's Width
		enemy2[i].dimensions.y			= ENEMY2_HEIGHT;				// Enemy2's Height
		enemy2[i].range_x				= ENEMY2_WIDTH  + Range_x;		// Enemy2's Horizontal range
		enemy2[i].range_y				= ENEMY2_HEIGHT + Range_y;		// Enemy2's Vertical range
		enemy2[i].Hp					= HP_RESET_2;					// Enemy2's Health
		enemy2[i].Max_Hp				= MAX_HP_RESET_2;				// Enemy2's Max Health
		enemy2[i].status				= TRUE;							// TRUE for alive, FALSE for dead
		// ---- Bullet ----
		bullet_enemy2[i].center.x		= enemy2[i].center.x;			// Bullet x position
		bullet_enemy2[i].center.y		= enemy2[i].center.y;			// Bullet y position
		bullet_enemy2[i].width			= BULLET_WIDTH;					// Bullet width
		bullet_enemy2[i].height			= BULLET_HEIGHT;				// Bullet height
		bullet_enemy2[i].speed			= BULLET_SPEED;					// Bullet speed
		bullet_enemy2[i].timer			= ENEMY2_TIMER;					// Bullet timer between each bullet
		bullet_enemy2[i].isTimerActive	= FALSE;						// Indicator for timer activeness
		bullet_enemy2[i].isTeleported	= FALSE;						// Indicator for teleporation
		bullet_enemy2[i].isShooting		= FALSE;						// Indicator to check whether bullet is still shooting

	}


}
/*!**************************************************************************************************
\brief
	Main Draw function of this source file
*******************************************************************************************************/
void enemies_draw() {

	enemy1_draw();			// Draws all enemy1
	enemy2_draw();			// Draws all enemy2
}

/*!**************************************************************************************************
\brief
	Main Update function of this source file
*******************************************************************************************************/
void enemies_update() {

	enemy1_update();		// Updates all enemy1
	enemy2_update();		// Updates all enemy2

}

/*!**************************************************************************************************
\brief
	Main Free function of this source file
*******************************************************************************************************/
void enemies_free() {
	// Nothing to be free
}

/*!**************************************************************************************************
\brief
	Main Unload function of this source file
*******************************************************************************************************/
void enemies_unload() {

	// Enemy1 texture unload
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		AEGfxTextureUnload(enemy1[i].texture);
	}
	// Enemy2 texture unload
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		AEGfxTextureUnload(enemy2[i].enemy2_fly1);
		AEGfxTextureUnload(enemy2[i].enemy2_fly2);
		AEGfxTextureUnload(enemy2[i].enemy2_dead);
	}
	// Bullet texture unload
	AEGfxTextureUnload(enemy2[0].bullet);
}


// ----------------------------------------------------------------------------------------  //
//	----------------					 ENEMY 1							---------------  //
// ----------------------------------------------------------------------------------------  // 

/*!**************************************************************************************************
\brief
	Checks for collision of enemy1 with player & bottom of the window
*******************************************************************************************************/
void enemy1_collision() {
	AEVec2 player_vec{ player.center.x , player.center.y };
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		if (enemy1[i].isDamageAllowed) {
			// decreases 1 player hp whenever player and enemy1 collide
			if (AETestRectToRect(&enemy1[i].center, ENEMY1_WIDTH, ENEMY1_HEIGHT, &player_vec, player.dimensions.x, player.dimensions.y)) {
				if (player.isShieldActive) {
					player.isShieldActive = FALSE;
				}
				else {
					--player.Hp;

					// Player's taking damage sfx
					AEAudioPlay(playerDamageAudio, soundGroup, 0.25f, 1.f, 0);
				}
				// disables damage temporarily once collided
				enemy1[i].isDamageAllowed = FALSE;
			}
		}

		else {
			// allows damage again 100 frames after player-enemy1 collision 
			if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
				enemy1[i].isDamageAllowed = TRUE;
			}
		}

		// bottom of screen
		if (enemy1[i].center.y < -WINDOWLENGTH_Y / 2.f + enemy1[i].dimensions.y / 2.f) {
			enemy1[i].Hp = 0;
		}
	}
}

/*!**************************************************************************************************
\brief
	Update x & y position of enemy1 with a given index
*******************************************************************************************************/
void enemy1_create(f32 x, f32 y, s32 index) {
	enemy1[index].center.x = x;
	enemy1[index].center.y = y;
}

/*!**************************************************************************************************
\brief
	Draws all enemies and their healthbar if an enemy1 instance have more than 0 health and not dead
*******************************************************************************************************/
void enemy1_draw() {

	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		// Draw enemies if alive
		if (enemy1[i].Hp > 0 && enemy1[i].status == TRUE) {

			// ----- Draw enemy -----
			AEGfxTextureSet(enemy1[i].texture, 0, 0);
			enemy1[i].GameObjects::RenderObject();

			// ----- Draw enemy HP bar -----
			enemy1[i].GameObjects::Render_HealthBar();
		}
	}
}

/*!**************************************************************************************************
\brief
	Update the game logic of enemy1. Movement of enemy1 based on frameElapsed of ongoing game.
	Provides XP for player if any enemy1 instance is dead.
*******************************************************************************************************/
void enemy1_update() {

	if (!isPaused) {
		for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {

			// total time that has passed since counter = 0 added into counter
			enemy1[i].movementCounter += AEFrameRateControllerGetFrameTime();

			// resets counter = 0 once counter has collected 200 frames worth of seconds
			if (enemy1[i].movementCounter > MAX_FRAME_MOVEMENT * AEFrameRateControllerGetFrameTime()) enemy1[i].movementCounter = NULL;

			// Moves left
			if (enemy1[i].movementCounter <= CHANGE_FRAME_MOVEMENT * AEFrameRateControllerGetFrameTime()) {
				enemy1[i].center.x -= ENEMY1_MOVEMENTX;
			}
			else { // Moves right
				enemy1[i].center.x += ENEMY1_MOVEMENTX;
			}

			// ------- XP for player -------
			if (enemy1[i].Hp <= 0 && enemy1[i].status == TRUE) {
				player.XP += static_cast<s32>(ENEMY1_DROPPED_XP);
				enemy1[i].status = FALSE;

				// Audio upon death
				AEAudioPlay(deathAudio, soundGroup, 1.f, 1.f, 0);
			}

		}
		// enemy collision with player & bottom of screen
		enemy1_collision();
	}
}

// ----------------------------------------------------------------------------------------  //
//	----------------					 ENEMY 2							---------------  //
// ----------------------------------------------------------------------------------------  // 
void enemy2_create(f32 x, f32 y, s32 index) {
	enemy2[index].center.x = x;
	enemy2[index].center.y = y;
	enemy2[index].status = TRUE;  // All alive at the start
}

void enemy2_draw() {
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		if (enemy2[i].Hp > 0 && enemy2[i].status == TRUE) {
			// ------  Rendering  ------
			AEGfxTextureSet(enemy2[i].enemy2_fly1, 0, 0);
			enemy2[i].GameObjects::RenderObject();

			// ------  Enemy2 bullets ------
			// If player is within range & left of enemy2
			if (bullet_enemy2[i].center.x >= (enemy2[i].center.x - enemy2[i].range_x) && bullet_enemy2[i].center.x < enemy2[i].center.x) {
				AEGfxTextureSet(enemy2[0].bullet, 0, 0);
				drawMesh(AEVec2{ bullet_enemy2[i].width, bullet_enemy2[i].height }, bullet_enemy2[i].center, NULL);
			}

			// ----- Draw enemy HP bar -----
			enemy2[i].GameObjects::Render_HealthBar();
		}
	}
}

void enemy2_update() {
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		// If enemy2 is not dead
		if (enemy2[i].Hp > 0 && enemy2[i].status == TRUE) {
			// ------  Enemy2 movement  ------
			if (!isPaused) {

				// Constantly increment movingTime
				enemy2[i].movingTime += AEFrameRateControllerGetFrameTime();

				// Reset movingTime to 0 when max movement is reached
				if (enemy2[i].movingTime > MAX_MOVEMENT) enemy2[i].movingTime = NULL;

				if (enemy2[i].movingTime <= CHANGE_MOVEMENT) enemy2[i].center.y += ENEMY2_MOVEMENTY;		// Move up
				else enemy2[i].center.y -= ENEMY2_MOVEMENTY;												// Move down
			}

			// ----- Enemy2 bullet -----
			// If player is within enemy2 range (ONLY WHEN PLAYER IS IN FRONT OF ENEMY2)
			if (player.center.x >= (enemy2[i].center.x - enemy2[i].range_x) && player.center.x <= enemy2[i].center.x &&
				player.center.y >= (enemy2[i].center.y - enemy2[i].range_y) && player.center.y <= (enemy2[i].center.y + enemy2[i].range_y)) {
				// --- Enable shooting ---
				bullet_enemy2[i].isShooting = TRUE;

				// If timer is over
				if (bullet_enemy2[i].isTimerActive == FALSE) {
					// ----- Movement of bullet from enemy2 -----
					if (bullet_enemy2[i].center.x >= (enemy2[i].center.x - enemy2[i].range_x)) {
						bullet_enemy2[i].center.x -= Bullet_Displacement_PerFrame;

						// --- Bullet sfx everytime bullet is reset ---
						if (bullet_enemy2[i].center.x == enemy2[i].center.x - Bullet_Displacement_PerFrame) {
							AEAudioPlay(bulletAudio, soundGroup, 0.5f, 1.f, 0);
						}
					}
					else {
						// --- Resets bullet ---
						bullet_enemy2[i].center.x = enemy2[i].center.x;
						bullet_enemy2[i].center.y = enemy2[i].center.y;
						bullet_enemy2[i].isTeleported = FALSE;

						// If player x within bullet delay range of enemy2
						if (player.center.x >= (enemy2[i].center.x - RangeToEnableBulletDelay) && player.center.x <= enemy2[i].center.x) {
							bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay
						}
					}
				}
			}
			else { // No longer in range
				// ---- Loops bullet ----
				if (bullet_enemy2[i].isShooting) {

					// ----- Movement of bullet from enemy2 -----
					if (bullet_enemy2[i].center.x >= (enemy2[i].center.x - enemy2[i].range_x)) {
						bullet_enemy2[i].center.x -= 5;
					}
					else {
						// --- Disable shooting ---
						bullet_enemy2[i].isShooting = FALSE;
					}
				}
				else {
					// --- Disable shooting ---
					bullet_enemy2[i].isShooting = FALSE;

					// --- Resets bullet ---
					bullet_enemy2[i].center.x = enemy2[i].center.x;
					bullet_enemy2[i].center.y = enemy2[i].center.y;
					bullet_enemy2[i].isTeleported = FALSE;
				}
			}
			// ----- Bullet collision with player -----
			AEVec2Set(&bullet_enemy2[i].center, bullet_enemy2[i].center.x, bullet_enemy2[i].center.y);
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &player.center, player.dimensions.x, player.dimensions.y)) {
				bullet_enemy2[i].center.x = enemy2[i].center.x;			// Reset bullet x
				bullet_enemy2[i].center.y = enemy2[i].center.y;			// Reset bullet y
				bullet_enemy2[i].isTimerActive = TRUE;					// Enable bullet delay

				// --- Disable shooting when player out of range of Enemy2 ---
				if (!(player.center.x >= (enemy2[i].center.x - enemy2[i].range_x) && player.center.x <= enemy2[i].center.x &&
					player.center.y >= (enemy2[i].center.y - enemy2[i].range_y) && player.center.y <= (enemy2[i].center.y + enemy2[i].range_y))) {
					bullet_enemy2[i].isShooting = FALSE;
				}

				if (player.isShieldActive) {
					player.isShieldActive = FALSE;
				}
				else {
					--player.Hp;

					// Player's taking damage sfx
					AEAudioPlay(playerDamageAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}

			// ----- Bullet collision with enemy2 -----
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy2[i].center, enemy2[i].dimensions.x, enemy2[i].dimensions.y) && bullet_enemy2[i].isTeleported) {
				bullet_enemy2[i].center.x = enemy2[i].center.x;
				bullet_enemy2[i].center.y = enemy2[i].center.y;
				bullet_enemy2[i].isTeleported = FALSE;
				bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

				--enemy2[i].Hp;

				// Upon taking damage
				AEAudioPlay(damageAudio, soundGroup, 1.f, 1.f, 0);
			}

			// ----- Bullet collision with enemy3 -----
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy3_a.center, enemy3_a.dimensions.x, enemy3_a.dimensions.y) && bullet_enemy2[i].isTeleported) {
				bullet_enemy2[i].center.x = enemy3_a.center.x;
				bullet_enemy2[i].center.y = enemy3_a.center.y;
				bullet_enemy2[i].isTeleported = FALSE;
				bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

				enemy3_a.Hp -= 2;

				// Upon taking damage
				AEAudioPlay(damageAudio, soundGroup, 1.f, 1.f, 0);
			}

			// ----- Bullet collision with enemy1 -----
			for (s32 j = 0; j < MAX_ENEMIES_1; ++j) {
				if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy1[j].center, enemy1[j].dimensions.x, enemy1[j].dimensions.y) && enemy1[j].status) {
					bullet_enemy2[i].center.x = enemy2[i].center.x;
					bullet_enemy2[i].center.y = enemy2[i].center.y;
					bullet_enemy2[i].isTeleported = FALSE;
					bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

					--enemy1[j].Hp;

					// Upon taking damage
					AEAudioPlay(damageAudio, soundGroup, 1.f, 1.f, 0);
				}
			}

			// ----- Resets bullet timer (Delay inbetween bullets) -----
			if (bullet_enemy2[i].isTimerActive == TRUE) {
				bullet_enemy2[i].timer -= static_cast<f32>(AEFrameRateControllerGetFrameTime());
			}
			if (bullet_enemy2[i].timer <= 0) {
				bullet_enemy2[i].timer = ENEMY2_TIMER;
				bullet_enemy2[i].isTimerActive = FALSE;
			}
		}

		// ------- XP for player -------
		if (enemy2[i].Hp <= 0 && enemy2[i].status == TRUE) {
			player.XP += static_cast<s32>(ENEMY2_DROPPED_XP);
			enemy2[i].status = FALSE;

			// Audio upon death
			AEAudioPlay(deathAudio, soundGroup, 1.f, 1.f, 0);
		}
	}
}
