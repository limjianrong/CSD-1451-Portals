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
#include "boss.hpp"
//#include "weapon_fire.hpp"
#include "GameState_Platformer.hpp"

#include <iostream>
// ----- Mesh & Texture -----
extern AEGfxVertexList* square_mesh;	// Created square mesh


// ----- Objects -----
extern Player_stats player;
Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2
Bullet bullet_enemy2[MAX_ENEMIES_2];	// Array of struct enemy2's bullet
extern bool isShieldActive;

// ----- Pause Menu -----
extern bool isPaused;

bool damage_allowed{ TRUE };	// Enemy1

//f32 g_dt;


void enemies_load() {

	// ---- Enemy1 Texture ----
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		enemy1[i].texture = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/spikeMan_stand.png");
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
	//enemy1_create(600, 90, 0);
	enemy1_create(-300, 90, 0);
	enemy1_create(2425, 590, 1);
	enemy1_create(6750, 1090, 2);

#ifdef DEBUG
	enemy1_create(-300, 90, 0);
	enemy2_create(-200, 90, 0);
#endif // DEBUG


	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {

		//enemy1[i].rotation = PI;					// Enemy1's Rotation
		enemy1[i].dimensions.x = ENEMY1_WIDTH;				// Enemy1's Width
		enemy1[i].dimensions.y = ENEMY1_HEIGHT;			// Enemy1's Height
		enemy1[i].Hp = 5;							// Enemy1's Health
		enemy1[i].Max_Hp = 5;						// Enemy1's Max Health
		enemy1[i].status = TRUE;					// TRUE for alive, FALSE for dead
		enemy1[i].movementCounter = 0.f;			// Enemy1's Movement Counter

	}


	// ------- Enemy 2 & bullets -------
	enemy2_create(3350, 900, 0); //y+150   6, 3350, 200, 11
	enemy2_create(3800, 875, 1);
	enemy2_create(3800, 925, 2);
	enemy2_create(3700, 250, 3);
	enemy2_create(3700, 300, 4);

	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {

		// ---- Enemy2 ----
		enemy2[i].dimensions.x = ENEMY2_WIDTH;				// Enemy2's Width
		enemy2[i].dimensions.y = ENEMY2_HEIGHT;			// Enemy2's Height
		enemy2[i].range_x = ENEMY2_WIDTH + 350;		// Enemy2's Horizontal range
		enemy2[i].range_y = ENEMY2_HEIGHT + 500;	// Enemy2's Vertical range
		enemy2[i].Hp = 5;							// Enemy2's Health
		enemy2[i].Max_Hp = 5;						// Enemy2's Max Health
		enemy2[i].status = TRUE;					// TRUE for alive, FALSE for dead
		// ---- Bullet ----
		bullet_enemy2[i].x = enemy2[i].center.x;	// Bullet x position
		bullet_enemy2[i].y = enemy2[i].center.y;	// Bullet y position
		bullet_enemy2[i].width = 20.0f;				// Bullet width
		bullet_enemy2[i].height = 20.0f;			// Bullet height
		bullet_enemy2[i].speed = 5.0f;				// Bullet speed
		bullet_enemy2[i].timer = ENEMY2_TIMER;		// Bullet timer between each bullet
		bullet_enemy2[i].isTimerActive = FALSE;		// Indicator for timer activeness
		bullet_enemy2[i].isTeleported = FALSE;		// Indicator for teleporation
		bullet_enemy2[i].isShooting = FALSE;		// Indicator to check whether bullet is still shooting
	}


}
/*!**************************************************************************************************
\brief
	Draws enemy if enemy HP is more than 0 to its updated position from function enemy_update
*******************************************************************************************************/
void enemies_draw() {

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
void enemies_update() {

	update_enemy1();		// Updates all enemy1

	enemy2_update();		// Updates all enemy2

	// Get Delta Time
	//g_dt = (f32)AEFrameRateControllerGetFrameTime();

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
			//if (AETestRectToRect(&enemy1[i].center, ENEMY1_WIDTH, ENEMY1_HEIGHT, &player_vec, player.width, player.height)) {
			if (CollisionIntersection_RectRect(enemy1[i], player)) {
				if (isShieldActive) {
					isShieldActive = FALSE;
				}
				else {
					--player.Hp;
				}
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

		// bottom of screen
		if (enemy1[i].center.y < -WINDOWLENGTH_Y / 2.f + enemy1[i].dimensions.y / 2.f) {
			//enemy1[i].status = FALSE;
			enemy1[i].Hp = 0;
		}
	}
}

void enemy1_create(f32 x, f32 y, s32 index) {
	enemy1[index].center.x = x;
	enemy1[index].center.y = y;
}

void enemy1_draw() {

	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		// Draw enemies if alive
		if (enemy1[i].Hp > 0 && enemy1[i].status == TRUE) {

			// ----- Draw enemy -----
			AEGfxTextureSet(enemy1[i].texture, 0, 0);
			drawMesh(AEVec2{ enemy1[i].dimensions.x, enemy1[i].dimensions.y }, enemy1[i].center, PI);
			// Set vector
			AEVec2Set(&enemy1[i].center, enemy1[i].center.x, enemy1[i].center.y);

			// ----- Draw enemy HP bar -----
			Render_HealthBar(static_cast<Enemy&>(enemy1[i]));

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
			if (enemy1[i].movementCounter <= 200 * AEFrameRateControllerGetFrameTime() && enemy1[i].movementCounter > 100 * AEFrameRateControllerGetFrameTime()) {
				enemy1[i].center.x += 1.0f;
			}
			// moves left when 0 < frames <= 100
			else if (enemy1[i].movementCounter <= 100 * AEFrameRateControllerGetFrameTime()) {
				enemy1[i].center.x -= 1.0f;
			}

			// ------- XP for player -------
			if (enemy1[i].Hp <= 0 && enemy1[i].status == TRUE) {
				player.XP += static_cast<s32>(ENEMY1_DROPPED_XP);
				enemy1[i].status = FALSE;
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
			// ------  Enemy2  ------
			// Set vector
			AEVec2Set(&enemy2[i].center, enemy2[i].center.x, enemy2[i].center.y);
			RenderEnemy(enemy2[i]);

			// ------  Enemy2 bullets ------
			// If player is within range & left of enemy2
			if (bullet_enemy2[i].x >= (enemy2[i].center.x - enemy2[i].range_x) && bullet_enemy2[i].x < enemy2[i].center.x) {
				AEGfxTextureSet(enemy2[0].bullet, 0, 0);
				drawMesh(AEVec2{ bullet_enemy2[i].width, bullet_enemy2[i].height }, bullet_enemy2[i].center, PI);
			}

			// ----- Draw enemy HP bar -----
			Render_HealthBar(static_cast<Enemy&>(enemy2[i]));
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

				// Reset movingTime to 0 when 4 is reached (Get 0-4)
				if (enemy2[i].movingTime > 4) enemy2[i].movingTime = 0.f;

				if (enemy2[i].movingTime <= 2) enemy2[i].center.y += 1.0f;		// Move up
				else enemy2[i].center.y -= 1.0f;								// Move down
			}

			// ----- Enemy2 bullet -----
			// If player is within enemy2 range (350x500 FOR NOW) (ONLY WHEN PLAYER IS IN FRONT OF ENEMY2)
			if (player.x >= (enemy2[i].center.x - enemy2[i].range_x) && player.x <= enemy2[i].center.x &&
				player.y >= (enemy2[i].center.y - enemy2[i].range_y) && player.y <= (enemy2[i].center.y + enemy2[i].range_y)) {
				// --- Enable shooting ---
				bullet_enemy2[i].isShooting = TRUE;

				// If timer is over
				if (bullet_enemy2[i].isTimerActive == FALSE) {
					// ----- Movement of bullet from enemy2 -----
					if (bullet_enemy2[i].x >= (enemy2[i].center.x - enemy2[i].range_x)) {
						bullet_enemy2[i].x -= 5;
					}
					else {
						// --- Resets bullet ---
						bullet_enemy2[i].x = enemy2[i].center.x;
						bullet_enemy2[i].y = enemy2[i].center.y;
						bullet_enemy2[i].isTeleported = FALSE;

						// If player x within 100 units of enemy2
						if (player.x >= (enemy2[i].center.x - 100.f) && player.x <= enemy2[i].center.x) {
							bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay
						}
					}
				}
			}
			else { // No longer in range
				// ---- Loops bullet ----
				if (bullet_enemy2[i].isShooting) {

					// ----- Movement of bullet from enemy2 -----
					if (bullet_enemy2[i].x >= (enemy2[i].center.x - enemy2[i].range_x)) {
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
					bullet_enemy2[i].x = enemy2[i].center.x;
					bullet_enemy2[i].y = enemy2[i].center.y;
					bullet_enemy2[i].isTeleported = FALSE;

				}
			}
			// ----- Bullet collision with player -----
			AEVec2Set(&bullet_enemy2[i].center, bullet_enemy2[i].x, bullet_enemy2[i].y);
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &player.center, player.width, player.height)) {
				bullet_enemy2[i].x = enemy2[i].center.x;			// Reset bullet x
				bullet_enemy2[i].y = enemy2[i].center.y;			// Reset bullet y
				bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay


				// --- Disable shooting when player out of range of Enemy2 ---
				if (!(player.x >= (enemy2[i].center.x - enemy2[i].range_x) && player.x <= enemy2[i].center.x &&
					player.y >= (enemy2[i].center.y - enemy2[i].range_y) && player.y <= (enemy2[i].center.y + enemy2[i].range_y))) {
					bullet_enemy2[i].isShooting = FALSE;
				}

				if (isShieldActive) {
					isShieldActive = FALSE;
				}
				else {
					--player.Hp;
				}
			}

			// ----- Bullet collision with enemy2 -----
			if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy2[i].center, enemy2[i].dimensions.x, enemy2[i].dimensions.y) && bullet_enemy2[i].isTeleported) {
				bullet_enemy2[i].x = enemy2[i].center.x;
				bullet_enemy2[i].y = enemy2[i].center.y;
				bullet_enemy2[i].isTeleported = FALSE;
				bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

				--enemy2[i].Hp;
			}

			// ----- Bullet collision with enemy1 -----
			for (s32 j = 0; j < MAX_ENEMIES_1; ++j) {
				if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &enemy1[j].center, enemy1[j].dimensions.x, enemy1[j].dimensions.y) && enemy1[j].status) {
					bullet_enemy2[i].x = enemy2[i].center.x;
					bullet_enemy2[i].y = enemy2[i].center.y;
					bullet_enemy2[i].isTeleported = FALSE;
					bullet_enemy2[i].isTimerActive = TRUE;		// Enable bullet delay

					--enemy1[j].Hp;
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
		}
	}
}

void RenderEnemy(Enemy2_stats enemy)
{
	AEMtx33Scale(&enemy.scale, enemy.dimensions.x, enemy.dimensions.y);
	AEMtx33Rot(&enemy.rotate, PI);
	AEMtx33Trans(&enemy.translate, enemy.center.x, enemy.center.y);
	AEMtx33Concat(&enemy.transform, &enemy.rotate, &enemy.scale);
	AEMtx33Concat(&enemy.transform, &enemy.translate, &enemy.transform);
	AEGfxSetTransform(enemy.transform.m);
	AEGfxTextureSet(enemy.enemy2_fly1, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}

void Render_HealthBar(Enemy& obj1) {

	// Draw enemies if alive
	if (obj1.Hp > 0 && obj1.status == TRUE) {
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(0, 0, 0, 1.f);
		drawMesh(AEVec2{ 80.f, 15.f }, AEVec2{ obj1.center.x, obj1.center.y + obj1.dimensions.y / 2.f }, PI);

		f32 health_percentage = ((float)obj1.Hp / (float)obj1.Max_Hp) * 100.f;
		/*if (health_percentage >= 80.f) {
			AEGfxSetTintColor(0.f, 1.f, 0.f, 1.f);
		}
		else if (health_percentage >= 40.f) {
			AEGfxSetTintColor(1.f, 1.f, 0.f, 1.f);
		}
		else {
			AEGfxSetTintColor(1.f, 0.f, 0.f, 1.f);
		}*/
		if (health_percentage >= 80.f) {
			AEGfxSetTintColor(0, 255, 0, 1.f);
		}
		else if (health_percentage >= 40.f) {
			AEGfxSetTintColor(255, 255, 0, 1.f);
		}
		else {
			AEGfxSetTintColor(255, 0, 0, 1.f);
		}
		drawMesh(AEVec2{ (float)obj1.Hp / (float)obj1.Max_Hp * 80.f , 15.f }, AEVec2{ (float)obj1.center.x - (((float)obj1.Max_Hp - (float)obj1.Hp) / (float)obj1.Max_Hp * 40.f), obj1.center.y + obj1.dimensions.y / 2.f }, PI);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	}
}
