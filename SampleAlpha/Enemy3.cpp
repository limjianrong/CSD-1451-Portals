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
Enemy3_stats enemy3_a; // enemy3 instance
bool enemy3_a_Dead, damage_allowed3{ TRUE }, going_left{ FALSE }; 

// ----- Player -----
extern Player_stats player;

// ----- Pause Menu -----
extern bool isPaused;

void enemy3_load() {
	enemy3 = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/wingMan3.png");

	enemy3_warning = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/powerup_wings.png");
}

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/

void enemy3_init() {

	// spawn 3000.f outside left edge of screen
	enemy3_a.x = -WINDOWLENGTH_X/2.f - 3000.f;
	// initialize y to 0
	enemy3_a.y = 0;
	
}

void draw_enemy3() {
	//draws enemy3 if alive
	if (enemy3_a.Hp > 0 && enemy3_a_Dead == FALSE) {
		
		AEMtx33Scale(&scale, ENEMY3_WIDTH, ENEMY3_HEIGHT);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, enemy3_a.x, enemy3_a.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy3, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		// set center vector
		AEVec2Set(&enemy3_a.center, enemy3_a.x, enemy3_a.y);

			// right edge of screen < enemy3 x position < right edge of screen + 900.f
		if (((enemy3_a.x > (WINDOWLENGTH_X/2.f + ENEMY3_WIDTH/2.f)+player.x )&& (enemy3_a.x < (WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f)+900.f+player.x)) 
			// player x is greater than initial screen center x = 0
			&& player.x > 0 
			// enemy3 currently heading left
			&& going_left) {
			

			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (player.x + WINDOWLENGTH_X/2.f) - WARNING_WIDTH/2.f, enemy3_a.y); // warning label drawn at right edge of screen
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
				// left edge of screen - 900.f < enemy 3 x position < left edge of screen
		else if ((enemy3_a.x < ((-WINDOWLENGTH_X / 2.f) - ENEMY3_WIDTH/2.f) +player.x) && (enemy3_a.x > ((-WINDOWLENGTH_X / 2.f) - ENEMY3_WIDTH/2.f)-900.f+player.x) 
			// player x is greater than initial screen center x = 0
			&& player.x > 0 
			// enemy 3 currently heading right
			&& !going_left ) {
			
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (player.x+( - WINDOWLENGTH_X / 2.f)) + WARNING_WIDTH/2.f, enemy3_a.y); // warning label drawn at left edge of screen
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
				// right edge of screen < enemy 3 x position < right edge of screen + 900.f
		else if ((enemy3_a.x > (WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f) && (enemy3_a.x < (WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f) + 900.f)) 
			// player x is lesser than or equal to initial screen center x = 0, camera has not started moving
			&& player.x <= 0 
			// enemy 3 currentlty heading left
			&& going_left) {
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (WINDOWLENGTH_X / 2.f) - WARNING_WIDTH / 2.f, enemy3_a.y); // warning label drawn at right edge of screen
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
				// left edge of screen - 900.f < enemy 3 x position < left edge of screen
		else if ((enemy3_a.x < ((-WINDOWLENGTH_X / 2.f) - ENEMY3_WIDTH/2.f)) && (enemy3_a.x > ((-WINDOWLENGTH_X / 2.f) - ENEMY3_WIDTH/2.f) - 900.f)
			// player x is lesser than or equal to initial screen center x = 0, camera has not started moving
			&& player.x <= 0 
			// enemy 3 currenly heading right
			&& !going_left) {
			
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (-WINDOWLENGTH_X / 2.f) + WARNING_WIDTH / 2.f, enemy3_a.y); // warning label drawn at left edge of screen
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}

		
	}
	// ------- XP for player -------
	else if (enemy3_a.Hp <= 0 && enemy3_a_Dead == FALSE) {
		player.XP += 30;
		enemy3_a_Dead = TRUE;
	}
}
/*
void enemy3_update(Player_stats* player) {
	Enemy3_stats* Enemy3_a = &enemy3_a;

	if (!isPaused) {
		// get 0-200
		s32 value = AEFrameRateControllerGetFrameCount() % 201;
		
		Enemy3_a->y = player->y;

		if (value <= 100) {

			if (Enemy3_a->x > player->x) {
				Enemy3_a->x += 10.f;
			}
			else {
				Enemy3_a->x += 10.f;
			}

		}
		else {
			Enemy3_a->y = player->y;

				if (Enemy3_a->x > player->x) {
					Enemy3_a->x -= 1.f;
				}
				else {
					Enemy3_a->x += 1.f;
				}
		}
	}
}
*/
void enemy3_update(Player_stats* player) {
	Enemy3_stats* Enemy3_a = &enemy3_a;
	if (!isPaused) {
		s32 value = AEFrameRateControllerGetFrameCount() % 3000;
		
		// moves right
		if (value <= 1500) {
			//enemy3 y position is determined (matches player y pos) when it is 900+ away from left/right edge of screen

				// right edge, player x <= initial screen center x
			if ((enemy3_a.x > WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f + 900.f && player->x <= 0)
				// left edge, player x <= initial screen center x
				|| (enemy3_a.x < -WINDOWLENGTH_X / 2.f - ENEMY3_WIDTH / 2.f - 900.f && player->x <= 0)
				// right edge, player x > initial screen center x
				|| (enemy3_a.x > WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f + 900.f + player->x && player->x > 0)
				// left edge, player x > initial screen center x
				|| (enemy3_a.x < -WINDOWLENGTH_X / 2.f - ENEMY3_WIDTH / 2.f - 900.f + player->x && player->x > 0)) {
				Enemy3_a->y = player->y;
			}
			
			Enemy3_a->x += 10.f;
			going_left = false;
		}

		// moves left
		else {
			//enemy3 y position is determined (matches player y pos) when it is more than 900.f away from left/right edge of screen

			// right edge, player x <= initial screen center x
			if ((enemy3_a.x > WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f + 900.f && player->x <= 0)
				// left edge, player x <= initial screen center x
				|| (enemy3_a.x < -WINDOWLENGTH_X / 2.f - ENEMY3_WIDTH / 2.f - 900.f && player->x <= 0)
				// right edge, player x > initial screen center x
				|| (enemy3_a.x > WINDOWLENGTH_X / 2.f + ENEMY3_WIDTH / 2.f + 900.f + player->x && player->x > 0)
				// left edge, player x > initial screen center x
				|| (enemy3_a.x < -WINDOWLENGTH_X / 2.f - ENEMY3_WIDTH / 2.f - 900.f + player->x && player->x > 0)) {
				Enemy3_a->y = player->y;
			}

		Enemy3_a->x -= 10.f;
			going_left = true;
		}
		

	}

}
void enemy3_free() {

}

void enemy3_unload() {
	// Enemy3 texture unload
	AEGfxTextureUnload(enemy3);
	AEGfxTextureUnload(enemy3_warning);
}

// enemy3-player collision
//void enemy3_collision(Player_stats* player) {
//	
//	AEVec2 player_vec{ player->x , player->y };
//
//	//s32 value = AEFrameRateControllerGetFrameCount() % 100;
//
//	if (damage_allowed3) {
//		if (AETestRectToRect(&enemy3_a.center, ENEMY3_WIDTH/75, ENEMY3_HEIGHT, &player_vec, PLAYER_WIDTH/75, PLAYER_HEIGHT)) {
//			player->Hp -= 3;
//		damage_allowed3 = FALSE;
//		}
//	}
//
//	else{
//		// damage allowed again after 100 frames has passed
//		if (AEFrameRateControllerGetFrameCount()%100 == 0) {
//			damage_allowed3 = TRUE;
//		}
//	} 
//}

void enemy3_collision() {
	AEVec2 player_vec{ player.x , player.y };
	
		if (damage_allowed3) {
			// decreases 1 player hp whenever player and enemy1 collide
			if (AETestRectToRect(&enemy3_a.center, ENEMY3_WIDTH/10, ENEMY3_HEIGHT/10, &player_vec, PLAYER_WIDTH/10, PLAYER_HEIGHT/10)) {
				player.Hp -= 3;
				// disables damage temporarily once collided
				damage_allowed3 = FALSE;
			}
		}

		else {
			// allows damage again 100 frames after player-enemy1 collision 
			if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
				damage_allowed3 = TRUE;
			}
		}
	
}
