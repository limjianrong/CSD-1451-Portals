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
AEGfxVertexList* enemy1_mesh;


// ----- Objects -----
extern Player_stats player;
Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2
Bullet bullet_enemy2[MAX_ENEMIES_2];	// Array of struct enemy2's bullet

f32 dist_enemy2bullet, dist_enemy2player;
bool damage_allowed{ TRUE };

// ----- Pause Menu -----
extern bool isPaused;

//f64 deltaTime{};
//f64 lastFrameTime{};

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

	// Saving the mesh (list of triangles) in enemy_mesh FOR NOW
	enemy1_mesh = create_Square_Mesh();
}
/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/
void enemies_init() {

	// ------- Enemy 1 -------
	enemy1_create(-200, -110, 0);
	enemy1_create(150, 190, 1);

	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {

		enemy1[i].rotation				= PI;					// Enemy1's Rotation
		enemy1[i].width					= ENEMY1_WIDTH;			// Enemy1's Width
		enemy1[i].height				= ENEMY1_HEIGHT;		// Enemy1's Height
		enemy1[i].Hp					= 5;					// Enemy1's Health
		enemy1[i].status				= TRUE;					// TRUE for alive, FALSE for dead
	}


	// ------- Enemy 2 & bullets -------
	enemy2_create(300, -120, 0);
	enemy2_create(300, -60, 1);
	enemy2_create(300, 0, 2);
	enemy2_create(300, 60, 3);
	enemy2_create(300, 120, 4);
	enemy2_create(300, 180, 5);
	enemy2_create(1750, 240, 6);

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
		bullet_enemy2[i].speed			= 5;					// Bullet speed
		bullet_enemy2[i].doesDamage		= FALSE;				// Indicator for dealing damage
		bullet_enemy2[i].isTeleported	= FALSE;				// Indicator for teleporation
		bullet_enemy2[i].isShooting		= FALSE;				// Indicator to check whether bullet is still shooting
	}


}
/*!**************************************************************************************************
\brief
	Draws enemy if enemy HP is more than 0 to its updated position from function enemy_update
*******************************************************************************************************/
void enemies_draw () {

	enemy1_draw();
	enemy2_draw();
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

	update_enemy1();
	//enemy1_collision(enemy1);

	enemy2_update();

}

void enemies_free() {

}

void enemies_unload () {
	//AEGfxMeshFree(enemy1_mesh);
	//AEGfxTextureUnload(enemy1Tex);
}


// ----------------------------------------------------------------------------------------  //
//	----------------					 ENEMY 1							---------------  //
// ----------------------------------------------------------------------------------------  // 

//void enemy1_collision(Enemy1_stats enemy1) {
//	AEVec2 player_vec{ player.x , player.y };
//	AEVec2 enemy_vec{ enemy1.x, enemy1.y };
//
//	if (damage_allowed) {
//		if (AETestRectToRect(&enemy_vec, ENEMY1_WIDTH, ENEMY1_HEIGHT, &player_vec, PLAYER_WIDTH, PLAYER_HEIGHT)) {
//			--player.Hp;
//			damage_allowed = FALSE;
//		}
//	}
//
//	else {
//		if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
//			damage_allowed = TRUE;
//		}
//	}
//}
void enemy1_create(f32 x, f32 y, s32 index) {
	enemy1[index].x = x;
	enemy1[index].y = y;
}

void enemy1_draw() {
	
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		if (enemy1[i].Hp > 0 && enemy1[i].status == TRUE) {
			//enemy1.x = update_enemy1(enemy1.x);

			AEMtx33Scale(&enemy1[i].scale, enemy1[i].width, enemy1[i].height);
			AEMtx33Rot(&enemy1[i].rotate, enemy1[i].rotation);
			AEMtx33Trans(&enemy1[i].translate, enemy1[i].x, enemy1[i].y);
			AEMtx33Concat(&enemy1[i].transform, &enemy1[i].rotate, &enemy1[i].scale);
			AEMtx33Concat(&enemy1[i].transform, &enemy1[i].translate, &enemy1[i].transform);
			AEGfxSetTransform(enemy1[i].transform.m);
			AEGfxTextureSet(enemy1[i].texture, 0, 0);
			AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);
			// Set vector
			AEVec2Set(&enemy1[i].center, enemy1[i].x, enemy1[i].y);

		}
	}
}

void update_enemy1() {

	if (!isPaused) {
		for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
			// get 0-200
			s32 value = AEFrameRateControllerGetFrameCount() % 201;

			if (value <= 100) {
				enemy1[i].x -= 1.0f;
			}
			else {
				enemy1[i].x += 1.0f;
			}

			// ------- XP for player -------
			if (enemy1[i].Hp <= 0 && enemy1[i].status == TRUE) {
				player.XP += 20;
				enemy1[i].status = FALSE;
			}

			///////////////
			/*
			f64 currentTime = AEFrameRateControllerGetFrameTime();
			deltaTime = currentTime - lastFrameTime;
			lastFrameTime = currentTime;

			f64 enemyMovement = 10.f * deltaTime;
			f64 enemyMovementDirection = (AEFrameRateControllerGetFrameCount() % 201 <= 100) ? -1.0f : 1.0f;
			x += enemyMovement * enemyMovementDirection;

			return static_cast<f32>(x);
			*/
		}
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
			AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);
			// Set vector
			AEVec2Set(&enemy2[i].center, enemy2[i].x, enemy2[i].y);

			// ------  Enemy2 bullets ------
			//if (dist_enemy2bullet < dist_enemy2player && isRunning == TRUE) {

			// If player is within range & left of enemy2
			if (bullet_enemy2[i].x >= (enemy2[i].x - enemy2[i].range_x) && bullet_enemy2[i].x < enemy2[i].x) {
				AEMtx33Scale(&enemy2[i].scale, bullet_enemy2[i].width, bullet_enemy2[i].height); // scaling it up
				AEMtx33Trans(&enemy2[i].translate, bullet_enemy2[i].x, bullet_enemy2[i].y); // shifts along x & y axis
				AEMtx33Rot(&enemy2[i].rotate, PI); // rotation
				AEMtx33Concat(&enemy2[i].transform, &enemy2[i].rotate, &enemy2[i].scale);
				AEMtx33Concat(&enemy2[i].transform, &enemy2[i].translate, &enemy2[i].transform);
				AEGfxSetTransform(enemy2[i].transform.m);
				AEGfxTextureSet(enemy2[0].bullet, 0, 0);
				AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);
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
				// get 0-200
				s32 value = AEFrameRateControllerGetFrameCount() % 401;

				if (value <= 200) {
					enemy2[i].y -= 0.5f;
				}
				else {
					enemy2[i].y += 0.5f;
				}
			}

			// ----- Enemy2 bullet -----
			// distance between enemy2 -> bullet, enemy2 -> player
			//dist_enemy2bullet = sqrt((bullet_enemy2.x - enemy2[i].x) * (bullet_enemy2.x - enemy2[i].x) + (bullet_enemy2.y - enemy2[i].y) * (bullet_enemy2.y - enemy2[i].y));
			//dist_enemy2player = sqrt((enemy2[i].x - player.x) * (enemy2[i].x - player.x) + (enemy2[i].y - player.y) * (enemy2[i].y - player.y));

			// If player is within enemy2 range (500x500 FOR NOW)
			/*if (player.x >= (enemy2.x - enemy2.range_x) && player.x <= (enemy2.x + enemy2.range_x) &&
				player.y >= (enemy2.y - enemy2.range_y) && player.y <= (enemy2.y + enemy2.range_y)) {*/

			// If player is within enemy2 range (300x500 FOR NOW) (ONLY WHEN PLAYER IS IN FRONT OF ENEMY2)
			if (player.x >= (enemy2[i].x - enemy2[i].range_x) && player.x <= enemy2[i].x &&
				player.y >= (enemy2[i].y - enemy2[i].range_y) && player.y <= (enemy2[i].y + enemy2[i].range_y)) {
				// --- Enable shooting ---
				bullet_enemy2[i].isShooting = TRUE;

				// ----- Movement of bullet from enemy2 -----
				if (bullet_enemy2[i].x >= (enemy2[i].x - enemy2[i].range_x)) {
					bullet_enemy2[i].x -= 5;
				}
				else {
					// --- Resets bullet ---
					bullet_enemy2[i].x = enemy2[i].x;
					bullet_enemy2[i].y = enemy2[i].y;
					bullet_enemy2[i].isTeleported = FALSE;
				}

				//// ---- Loops bullet ----
				//if (dist_enemy2bullet < dist_enemy2player && isRunning == TRUE) {

			}
			else { // No longer in range
				// ---- Loops bullet ----
				//if (dist_enemy2bullet < dist_enemy2player && isRunning == TRUE) {
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
				bullet_enemy2[i].x = enemy2[i].x;
				bullet_enemy2[i].y = enemy2[i].y;
				
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
				--enemy2[i].Hp;
			}

			// ----- Bullet collision with enemy1 -----
			for (s32 j = 0; j < MAX_ENEMIES_1; ++j) {
				if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy1[j].center, enemy1[j].width, enemy1[j].height) && enemy1[j].status) {
					bullet_enemy2[i].x = enemy2[i].x;
					bullet_enemy2[i].y = enemy2[i].y;
					bullet_enemy2[i].isTeleported = FALSE;
					--enemy1[j].Hp;
				}
			}
		}

		// ------- XP for player -------
		if (enemy2[i].Hp <= 0 && enemy2[i].status == TRUE) {
			player.XP += 20;
			enemy2[i].status = FALSE;
		}
	}
}
