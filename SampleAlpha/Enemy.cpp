/*!**************************************************************************************************
\file     Enemy.cpp
\author   Lee Zhi Yee & Lin ZhaoZhi
\par      DP email: zhiyee.l@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file implements the functions used to initialize and update enemy.

  The functions include:
  - enemy_init
	Load texture, initialise mesh

  - draw_enemy
	Draws enemy if its HP is more than 0

  - update_enemy
	Returns the updated position of enemy
*****************************************************************************************************/
#include "AEEngine.h"
#include <AEGraphics.h>
#include <ctime>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Utilities.hpp"
#include "weapon_fire.hpp"
#include "GameState_Platformer.hpp"

#include <iostream>
// ----- Mesh & Texture -----
extern AEGfxVertexList* square_mesh;	// Created square mesh


// ----- Objects -----
extern Player_stats player;
Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2
Bullet bullet_enemy2[MAX_ENEMIES_2];	// Array of struct enemy2's bullet

// ----- Pause Menu -----
extern bool isPaused;

bool damage_allowed{ TRUE };	// Enemy1



void enemies_load() {

	// ---- Enemy1 Texture ----
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		enemy1[i].texture = AEGfxTextureLoad("Assets/enemy.png");
	}

	// ---- Enemy2 Texture ----
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		enemy2[i].enemy2_fly1 = AEGfxTextureLoad("Assets/bat.png");
		enemy2[i].enemy2_fly2 = AEGfxTextureLoad("Assets/bat_fly.png");
		enemy2[i].enemy2_dead = AEGfxTextureLoad("Assets/bat_dead.png");
	}
	enemy2[0].bullet = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/gold_1.png");
}

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/
void enemies_init() {

	// ------- Enemy 1 -------
	//enemy1_create(-200, -110, 0);
	//enemy1_create(150, 190, 1);
	enemy1_create(625, 80, 0);
	enemy1_create(1950, 230, 1);

	enemy1_create(200, 100, 2);

	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {

		enemy1[i].rotation				= PI;					// Enemy1's Rotation
		enemy1[i].width					= ENEMY1_WIDTH;			// Enemy1's Width
		enemy1[i].height				= ENEMY1_HEIGHT;		// Enemy1's Height
		enemy1[i].Hp					= 5;					// Enemy1's Health
		enemy1[i].status				= TRUE;					// TRUE for alive, FALSE for dead
		enemy1[i].movementCounter		= 0.f;					// Enemy1's Movement Counter

	}


	// ------- Enemy 2 & bullets -------
	//enemy2_create(300, -120, 0);
	//enemy2_create(300, -60, 1);
	//enemy2_create(300, 0, 2);
	//enemy2_create(300, 60, 3);
	//enemy2_create(300, 120, 4);
	//enemy2_create(300, 180, 5);
	//enemy2_create(1750, 240, 6);
	enemy2_create(2800, 0, 0);

	enemy2_create(100, 0, 1);

	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {

		// ---- Enemy2 ----
		enemy2[i].rotation				= PI;					// Enemy2's Rotation
		enemy2[i].width					= ENEMY2_WIDTH;			// Enemy2's Width
		enemy2[i].height				= ENEMY2_HEIGHT;		// Enemy2's Height
		enemy2[i].range_x				= ENEMY2_WIDTH + 350;	// Enemy2's Horizonal range
		enemy2[i].range_y				= ENEMY2_HEIGHT + 500;	// Enemy2's Vertical range
		enemy2[i].Hp					= 5;					// Enemy2's Health
		enemy2[i].status				= TRUE;					// TRUE for alive, FALSE for dead
		// ---- Bullet ----
		bullet_enemy2[i].x				= enemy2[i].x;			// Bullet x position
		bullet_enemy2[i].y				= enemy2[i].y;			// Bullet y position
		bullet_enemy2[i].width			= 20.0f;				// Bullet width
		bullet_enemy2[i].height			= 20.0f;				// Bullet height
		bullet_enemy2[i].speed			= 5.0f;					// Bullet speed
		bullet_enemy2[i].timer			= ENEMY2_TIMER;			// Bullet timer between each bullet
		bullet_enemy2[i].isTimerActive	= FALSE;				// Indicator for timer activeness
		bullet_enemy2[i].isTeleported	= FALSE;				// Indicator for teleporation
		bullet_enemy2[i].isShooting		= FALSE;				// Indicator to check whether bullet is still shooting
	}


}
/*!**************************************************************************************************
\brief
	Draws enemy if enemy HP is more than 0 to its updated position from function enemy_update
*******************************************************************************************************/
void enemies_draw () {

	enemy1_draw();		// Draws all enemy1
	enemy2_draw();		// Draws all enemy2
}

/*!**************************************************************************************************
\brief
	Calculates current enemy position based on total number of frames elapsed.

\param[in] EnemyCenter
	x coordinate of ememy

\return
	new enemy x position
*******************************************************************************************************/
void enemies_update () {

	update_enemy1();		// Updates all enemy1

	enemy2_update();		// Updates all enemy2

}

void enemies_free() {

}

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

void enemy1_collision() {
	AEVec2 player_vec{ player.x , player.y };
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		if (damage_allowed) {
			// decreases 1 player hp whenever player and enemy1 collide
			if (AETestRectToRect(&enemy1[i].center, ENEMY1_WIDTH, ENEMY1_HEIGHT, &player_vec, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				--player.Hp;
				// disables damage temporarily once collided
				damage_allowed = FALSE;
			}
		}

		else {
			// allows damage again 100 frames after player-enemy1 collision 
			if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
				damage_allowed = TRUE;
			}
		}
	}
}

void enemy1_create(f32 x, f32 y, s32 index) {
	enemy1[index].x = x;
	enemy1[index].y = y;
}

void enemy1_draw() {
	
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		// Draw enemies if alive
		if (enemy1[i].Hp > 0 && enemy1[i].status == TRUE) {

			AEMtx33Scale(&enemy1[i].scale, enemy1[i].width, enemy1[i].height);
			AEMtx33Rot(&enemy1[i].rotate, enemy1[i].rotation);
			AEMtx33Trans(&enemy1[i].translate, enemy1[i].x, enemy1[i].y);
			AEMtx33Concat(&enemy1[i].transform, &enemy1[i].rotate, &enemy1[i].scale);
			AEMtx33Concat(&enemy1[i].transform, &enemy1[i].translate, &enemy1[i].transform);
			AEGfxSetTransform(enemy1[i].transform.m);
			AEGfxTextureSet(enemy1[i].texture, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
			// Set vector
			AEVec2Set(&enemy1[i].center, enemy1[i].x, enemy1[i].y);

		}
	}
}

void update_enemy1() {
	
	if (!isPaused) {
		for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
			
			// total time that has passed since counter = 0 added into counter
			enemy1[i].movementCounter += AEFrameRateControllerGetFrameTime();
			
			// resets counter = 0 once counter has collected 200 frames worth of seconds
			if (enemy1[i].movementCounter > 200 * AEFrameRateControllerGetFrameTime()) enemy1[i].movementCounter = 0.f;
			
			// moves right when 100 < frames <= 200 
			if (enemy1[i].movementCounter <= 200*AEFrameRateControllerGetFrameTime() && enemy1[i].movementCounter > 100 * AEFrameRateControllerGetFrameTime()) {
				enemy1[i].x += 1.0f;
			}
			// moves left when 0 < frames <= 100
			else if (enemy1[i].movementCounter <= 100 * AEFrameRateControllerGetFrameTime()) {
				enemy1[i].x -= 1.0f;
			}

			// ------- XP for player -------
			if (enemy1[i].Hp <= 0 && enemy1[i].status == TRUE) {
				player.XP += ENEMY1_DROPPED_XP;
				enemy1[i].status = FALSE;
			}

		}
		// enemy collision with player
		enemy1_collision();
	}
}

// ----------------------------------------------------------------------------------------  //
//	----------------					 ENEMY 2							---------------  //
// ----------------------------------------------------------------------------------------  // 
void enemy2_create (f32 x, f32 y, s32 index) {
	enemy2[index].x = x;
	enemy2[index].y = y;
	enemy2[index].status = TRUE;  // All alive at the start
}

void enemy2_draw () {
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		if (enemy2[i].Hp > 0 && enemy2[i].status == TRUE) {
			// ------  Enemy2  ------
			AEMtx33Scale(&enemy2[i].scale, enemy2[i].width, enemy2[i].height);
			AEMtx33Rot(&enemy2[i].rotate, enemy2[i].rotation);
			AEMtx33Trans(&enemy2[i].translate, enemy2[i].x, enemy2[i].y);
			AEMtx33Concat(&enemy2[i].transform, &enemy2[i].rotate, &enemy2[i].scale);
			AEMtx33Concat(&enemy2[i].transform, &enemy2[i].translate, &enemy2[i].transform);
			AEGfxSetTransform(enemy2[i].transform.m);
			AEGfxTextureSet(enemy2[i].enemy2_fly1, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
			// Set vector
			AEVec2Set(&enemy2[i].center, enemy2[i].x, enemy2[i].y);

			// ------  Enemy2 bullets ------
			// If player is within range & left of enemy2
			if (bullet_enemy2[i].x >= (enemy2[i].x - enemy2[i].range_x) && bullet_enemy2[i].x < enemy2[i].x) {
				AEMtx33Scale(&enemy2[i].scale, bullet_enemy2[i].width, bullet_enemy2[i].height); // scaling it up
				AEMtx33Trans(&enemy2[i].translate, bullet_enemy2[i].x, bullet_enemy2[i].y); // shifts along x & y axis
				AEMtx33Rot(&enemy2[i].rotate, PI); // rotation
				AEMtx33Concat(&enemy2[i].transform, &enemy2[i].rotate, &enemy2[i].scale);
				AEMtx33Concat(&enemy2[i].transform, &enemy2[i].translate, &enemy2[i].transform);
				AEGfxSetTransform(enemy2[i].transform.m);
				AEGfxTextureSet(enemy2[0].bullet, 0, 0);
				AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
}

void enemy2_update () {
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		// If enemy2 is not dead
		if (enemy2[i].Hp > 0 && enemy2[i].status == TRUE) {
			// ------  Enemy2 movement  ------
			if (!isPaused) {

				// Constantly increment movingTime
				enemy2[i].movingTime += AEFrameRateControllerGetFrameTime();

				// Reset movingTime to 0 when 4 is reached (Get 0-4)
				if (enemy2[i].movingTime > 4) enemy2[i].movingTime = 0.f;

				if (enemy2[i].movingTime <= 2) enemy2[i].y += 1.0f;		// Move up
				else enemy2[i].y -= 1.0f;								// Move down
			}

			// ----- Enemy2 bullet -----
			// If player is within enemy2 range (350x500 FOR NOW) (ONLY WHEN PLAYER IS IN FRONT OF ENEMY2)
			if (player.x >= (enemy2[i].x - enemy2[i].range_x) && player.x <= enemy2[i].x &&
				player.y >= (enemy2[i].y - enemy2[i].range_y) && player.y <= (enemy2[i].y + enemy2[i].range_y)) {
				// --- Enable shooting ---
				bullet_enemy2[i].isShooting = TRUE;

				// If timer is over
				if (bullet_enemy2[i].isTimerActive == FALSE) {
					// ----- Movement of bullet from enemy2 -----
					if (bullet_enemy2[i].x >= (enemy2[i].x - enemy2[i].range_x)) {
						bullet_enemy2[i].x -= 5;
					}
					else {
						// --- Resets bullet ---
						bullet_enemy2[i].x = enemy2[i].x;
						bullet_enemy2[i].y = enemy2[i].y;
						bullet_enemy2[i].isTeleported = FALSE;

						// If player x within 100 units of enemy2
						if (player.x >= (enemy2[i].x - 100) && player.x <= enemy2[i].x) {
							bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay
						}
					}
				}

			}
			else { // No longer in range
				// ---- Loops bullet ----
				if (bullet_enemy2[i].isShooting) {

					// ----- Movement of bullet from enemy2 -----
					if (bullet_enemy2[i].x >= (enemy2[i].x - enemy2[i].range_x)) {
						bullet_enemy2[i].x -= 5;
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
					bullet_enemy2[i].x = enemy2[i].x;
					bullet_enemy2[i].y = enemy2[i].y;
					bullet_enemy2[i].isTeleported = FALSE;

				}
			}
			// ----- Bullet collision with player -----
			AEVec2Set(&bullet_enemy2[i].center, bullet_enemy2[i].x, bullet_enemy2[i].y);
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &player.center, player.width, player.height)) {
				bullet_enemy2[i].x = enemy2[i].x;			// Reset bullet x
				bullet_enemy2[i].y = enemy2[i].y;			// Reset bullet y
				bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

				
				// --- Disable shooting when player out of range of Enemy2 ---
				if (!(player.x >= (enemy2[i].x - enemy2[i].range_x) && player.x <= enemy2[i].x &&
					player.y >= (enemy2[i].y - enemy2[i].range_y) && player.y <= (enemy2[i].y + enemy2[i].range_y))) {
					bullet_enemy2[i].isShooting = FALSE;
				}
				--player.Hp;
			}

			// ----- Bullet collision with enemy2 -----
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy2[i].center, enemy2[i].width, enemy2[i].height) && bullet_enemy2[i].isTeleported) {
				bullet_enemy2[i].x = enemy2[i].x;
				bullet_enemy2[i].y = enemy2[i].y;
				bullet_enemy2[i].isTeleported = FALSE;
				bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

				--enemy2[i].Hp;
			}

			// ----- Bullet collision with enemy1 -----
			for (s32 j = 0; j < MAX_ENEMIES_1; ++j) {
				if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy1[j].center, enemy1[j].width, enemy1[j].height) && enemy1[j].status) {
					bullet_enemy2[i].x = enemy2[i].x;
					bullet_enemy2[i].y = enemy2[i].y;
					bullet_enemy2[i].isTeleported = FALSE;
					bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

					--enemy1[j].Hp;
				}
			}

			// ----- Resets bullet timer (Delay inbetween bullets) -----
			if (bullet_enemy2[i].isTimerActive == TRUE) {
				bullet_enemy2[i].timer -= AEFrameRateControllerGetFrameTime();
			}
			if (bullet_enemy2[i].timer <= 0) {
				bullet_enemy2[i].timer = ENEMY2_TIMER;
				bullet_enemy2[i].isTimerActive = FALSE;
			}
		}

		// ------- XP for player -------
		if (enemy2[i].Hp <= 0 && enemy2[i].status == TRUE) {
			player.XP += ENEMY2_DROPPED_XP;
			enemy2[i].status = FALSE;
		}
	}
}
