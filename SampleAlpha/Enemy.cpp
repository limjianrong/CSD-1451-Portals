/*!**************************************************************************************************
\file     Enemy.cpp
\author   Lee Zhi Yee
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

// ----- Mesh & Texture -----
AEGfxTexture* enemy1Tex;
AEGfxVertexList* enemy1_mesh;
extern AEMtx33 scale, rotate, translate, transform; // TRS

// ----- Objects -----
extern Player_stats player;
Enemy1_stats enemy1;
Enemy2_stats enemy2;
Bullet bullet_enemy2;
extern Bullet bullet;

f32 dist_enemy2bullet, dist_enemy2player;
bool enemy1_Dead{ FALSE }, damage_allowed{ TRUE };
static bool isRunning = FALSE;

// ----- Pause Menu -----
extern bool isPaused;

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/
void enemy_init() {
	
	// ---- Enemy1 Texture ----
	enemy1Tex = AEGfxTextureLoad("Assets/enemy.png");

	// ---- Enemy2 Texture ----
	enemy2.enemy2_fly1 = AEGfxTextureLoad("Assets/characters/Enemy sprites/bat.png");
	enemy2.enemy2_fly2 = AEGfxTextureLoad("Assets/characters/Enemy sprites/bat_fly.png");
	enemy2.enemy2_dead = AEGfxTextureLoad("Assets/characters/Enemy sprites/bat_dead.png");

	// Saving the mesh (list of triangles) in enemy_mesh
	enemy1_mesh = create_Square_Mesh();


	// FOR NOW ONLY
	enemy1.x = -300.0f;
	enemy1.y = -110.0f;

	enemy2.x = 50.0f;
	enemy2.y = 50.0f;
}

/*!**************************************************************************************************
\brief
	Draws enemy if enemy HP is more than 0 to its updated position from function enemy_update
*******************************************************************************************************/
void draw_enemy() {

	// No idea why this is required
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// ------  Enemy1  ------
	if (enemy1.Hp > 0 && enemy1_Dead == FALSE) {

		AEMtx33Scale(&scale, enemy1.width, enemy1.height);
		AEMtx33Rot(&rotate, enemy1.rotation);
		AEMtx33Trans(&translate, enemy1.x, enemy1.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy1Tex, 0, 0);
		AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);
		// Set vector
		AEVec2Set(&enemy1.center, enemy1.x, enemy1.y);
	}
	// ------- XP for player -------
	else if (enemy1.Hp <= 0 && enemy1_Dead == FALSE) {
		player.XP += 10;
		enemy1_Dead = TRUE;
	}


	// ------  Enemy2  ------
	AEMtx33Scale(&scale, enemy2.width, enemy2.height);
	AEMtx33Rot(&rotate, enemy2.rotation);
	AEMtx33Trans(&translate, enemy2.x, enemy2.y);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(enemy2.enemy2_fly1, 0, 0);
	AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);
	// Set vector
	AEVec2Set(&enemy2.center, enemy1.x, enemy1.y);

	// ------  Enemy2 bullets ------
	if (dist_enemy2bullet < dist_enemy2player && isRunning == TRUE) {
		AEMtx33Scale(&scale, bullet_enemy2.width, bullet_enemy2.height); // scaling it up
		AEMtx33Trans(&translate, bullet_enemy2.x, bullet_enemy2.y); // shifts along x & y axis
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(bullet.bulletTex, 0, 0);
		AEGfxMeshDraw(bullet.shootMesh, AE_GFX_MDM_TRIANGLES);
	}
}

/*!**************************************************************************************************
\brief
	Calculates current enemy position based on total number of frames elapsed.

\param[in] EnemyCenter
	x coordinate of ememy

\return
	new enemy x position
*******************************************************************************************************/
void update_enemy () {
	
	// ------  Enemy1 & Enemy2  ------
	if (!isPaused) {
		// get 0-200
		s32 value = AEFrameRateControllerGetFrameCount() % 201;

		if (value <= 100) {
			enemy1.x -= 1.0f;
			enemy2.x -= 1.0f;
		}
		else {
			enemy1.x += 1.0f;
			enemy2.x += 1.0f;
		}
	}

	// ----- Enemy2 bullet -----
	// distance between enemy2 -> bullet, enemy2 -> player
	dist_enemy2bullet = sqrt((bullet_enemy2.x - enemy2.x) * (bullet_enemy2.x - enemy2.x) + (bullet_enemy2.y - enemy2.y) * (bullet_enemy2.y - enemy2.y));
	dist_enemy2player = sqrt((enemy2.x - player.x) * (enemy2.x - player.x) + (enemy2.y - player.y) * (enemy2.y - player.y));

	// If player is within enemy2 range (100x100 FOR NOW)
	if (player.x >= (enemy2.x - enemy2.range_x) && player.x <= (enemy2.x + enemy2.range_x) &&
		player.y >= (enemy2.y - enemy2.range_y) && player.y <= (enemy2.y + enemy2.range_y)) {
		// --- Enable shooting ---
		isRunning = TRUE;
		// ---- Loops bullet ----
		if (dist_enemy2bullet < dist_enemy2player && isRunning == TRUE) {

			// ----- Movement of bullet from enemy2 to player -----
			bullet_enemy2.x -= 5;
			bullet_enemy2.y = enemy2.y;
		}
		else {
			// --- Resets bullet ---
			bullet_enemy2.x = enemy2.x;
		}
	}
	else { // No longer in range of boss
		// ---- Loops bullet ----
		if (dist_enemy2bullet < dist_enemy2player && isRunning == TRUE) {

			// ----- Movement of bullet from boss to player -----
			bullet_enemy2.x -= 5;
			bullet_enemy2.y = enemy2.y;
		}
		else {
			// --- Disable shooting ---
			isRunning = FALSE;
			// --- Resets bullet ---
			bullet_enemy2.x = enemy2.x;
		}
	}
	// ----- Bullet collision with boss -----
	if (AETestRectToRect(&bullet_enemy2.center, bullet_enemy2.width, bullet_enemy2.height, &enemy2.center, enemy2.width, enemy2.height) && bullet_enemy2.isTeleported) {
		bullet_enemy2.x = enemy2.x;
		bullet_enemy2.y = enemy2.y;
		bullet_enemy2.isTeleported = FALSE;
		--enemy2.Hp;
	}

}

void unload_enemy() {
	//AEGfxMeshFree(enemy1_mesh);
	//AEGfxTextureUnload(enemy1Tex);
}

// zh 
void enemy_collision(Player_stats* player) {
	AEVec2 player_vec{ player->x , player->y };

	if (damage_allowed) {
		if (AETestRectToRect(&enemy1.center, ENEMY1_WIDTH, ENEMY1_HEIGHT, &player_vec, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			--player->Hp;
			damage_allowed = FALSE;
		} 
	}

	else {
		if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
			damage_allowed = TRUE;
		}
	} 
}