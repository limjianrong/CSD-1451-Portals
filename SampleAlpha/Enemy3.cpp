#include "AEEngine.h"
#include <AEGraphics.h>
#include "Player.hpp"
#include "Utilities.hpp"
#include "Enemy3.hpp"





// ----- Mesh & Texture -----
AEGfxTexture* enemy3;
extern AEGfxVertexList* square_mesh;	// Created square mesh

AEGfxTexture* enemy3_warning;
extern AEMtx33 scale, rotate, translate, transform; // TRS

// ----- Enemy -----
Enemy3_stats enemy3_a;
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

	// spawn outside screen
	enemy3_a.x = -WINDOWLENGTH_X/2 - 1000.f;
	enemy3_a.y = 0;
	
}

void draw_enemy3() {

	if (enemy3_a.Hp > 0 && enemy3_a_Dead == FALSE) {
		
		AEMtx33Scale(&scale, ENEMY3_WIDTH, ENEMY3_HEIGHT);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, enemy3_a.x, enemy3_a.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy3, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		AEVec2Set(&enemy3_a.center, enemy3_a.x, enemy3_a.y);


		if (((enemy3_a.x > (WINDOWLENGTH_X/2 + ENEMY3_WIDTH/2)+player.x )&& (enemy3_a.x < (WINDOWLENGTH_X / 2 + ENEMY3_WIDTH / 2))+150.f+player.x) && player.x > 0 && going_left) {
			

			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (player.x + WINDOWLENGTH_X/2) - WARNING_WIDTH/2, enemy3_a.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
		else if ((enemy3_a.x < ((-WINDOWLENGTH_X / 2) - ENEMY3_WIDTH) +player.x) && (enemy3_a.x > ((-WINDOWLENGTH_X / 2) - ENEMY3_WIDTH)-150.f+player.x) && player.x > 0 && !going_left ) {
			
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (player.x+( - WINDOWLENGTH_X / 2)) + WARNING_WIDTH/2, enemy3_a.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
		else if ((enemy3_a.x > (WINDOWLENGTH_X / 2 + ENEMY3_WIDTH / 2) && (enemy3_a.x < (WINDOWLENGTH_X / 2 + ENEMY3_WIDTH / 2)) + 150.f) && player.x <= 0 && going_left) {
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (WINDOWLENGTH_X / 2) - WARNING_WIDTH / 2, enemy3_a.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
		else if ((enemy3_a.x < ((-WINDOWLENGTH_X / 2) - ENEMY3_WIDTH)) && (enemy3_a.x > ((-WINDOWLENGTH_X / 2) - ENEMY3_WIDTH) - 150.f) && player.x <= 0 && !going_left) {
			
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, (-WINDOWLENGTH_X / 2) + WARNING_WIDTH / 2, enemy3_a.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}

		// updates enemy position
		//enemy3_update(&enemy3_a, &player);
		////////////////////////////////////////////
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
		s32 value = AEFrameRateControllerGetFrameCount() % 1000;
		
		if (value <= 500) {
			//f32 const x = enemy3_a.x;
			if ((enemy3_a.x > WINDOWLENGTH_X / 2 + ENEMY3_WIDTH / 2 +150.f && player->x <= 0) || (enemy3_a.x < -WINDOWLENGTH_X / 2 - ENEMY3_WIDTH / 2-150.f && player->x <= 0) || (enemy3_a.x > WINDOWLENGTH_X/2 + ENEMY3_WIDTH/2 +150.f +player->x && player->x > 0) || (enemy3_a.x < -WINDOWLENGTH_X / 2 - ENEMY3_WIDTH / 2 -150.f +player->x && player->x > 0)) {
				Enemy3_a->y = player->y;
			}
			Enemy3_a->x += 10.f;
			going_left = false;
			}

			else {
			if ((enemy3_a.x > WINDOWLENGTH_X / 2 + ENEMY3_WIDTH / 2+ 150.f && player->x <= 0) || (enemy3_a.x < -WINDOWLENGTH_X / 2 - ENEMY3_WIDTH / 2-150.f && player->x <= 0) || (enemy3_a.x > WINDOWLENGTH_X / 2 + ENEMY3_WIDTH/2 +150.f + player->x && player->x > 0) || (enemy3_a.x < -WINDOWLENGTH_X / 2 - ENEMY3_WIDTH / 2 -150.f+ player->x && player->x > 0)) {
				Enemy3_a->y = player->y;
			}
			Enemy3_a->x -= 10.f;
				going_left = true;
			}
		

	}

}

void enemy3_collision(Player_stats* player) {
	AEVec2 player_vec{ player->x , player->y };

	s32 value = AEFrameRateControllerGetFrameCount() % 100;

	if (damage_allowed3) {
		if (AETestRectToRect(&enemy3_a.center, ENEMY3_WIDTH/75, ENEMY3_HEIGHT, &player_vec, PLAYER_WIDTH/75, PLAYER_HEIGHT)) {
			player->Hp -= 3;
		damage_allowed3 = FALSE;
		}
	}

	else{
		if (value < 51) {
			damage_allowed3 = TRUE;
		}
	} 
}

