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
#include "GameState_Platformer.hpp"

// ----- Mesh & Texture -----
AEGfxTexture* enemy1;
AEGfxVertexList* enemy1_mesh;

// ----- Enemy -----
Enemy1_stats enemy1_a, enemy1_b;
bool enemy1_a_Dead, enemy1_b_Dead, damage_allowed{ TRUE };

// ----- Player -----
extern Player_stats player;

// ----- Pause Menu -----
extern bool isPaused;

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/
void enemy_init() {

	enemy1 = AEGfxTextureLoad("Assets/enemy.png");
	// Saving the mesh (list of triangles) in enemy_mesh
	enemy1_mesh = create_Square_Mesh();


	// FOR NOW ONLY
	enemy1_a.x = -300.0f;
	enemy1_a.y = -110.0f;

	enemy1_b.x = 100.0f;
	enemy1_b.y = 40.0f;
}

/*!**************************************************************************************************
\brief
	Draws enemy if enemy HP is more than 0 to its updated position from function enemy_update
*******************************************************************************************************/
void draw_enemy() {

	if (enemy1_a.Hp > 0 && enemy1_a_Dead == FALSE) {
		// Tell the engine to get ready to draw something with texture.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can 
		// display the full range of colors (default is black).
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND
		// This will allow transparency.
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		//AEGfxSetTransparency(1.0f);
		// Set the texture
		AEGfxTextureSet(enemy1, 0, 0);
		// Create a scale matrix
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 60.f, 80.f);
		// Create a rotation matrix that rotates by 90 degrees
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);
		// Create a translation matrix that translates by
		// 100 in the x-axis and 100 in the y-axis
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, enemy1_a.x, enemy1_a.y);
		// Concatenate the matrices (TRS)
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to apply onto the vertices 
		// of the mesh that we are choose to draw in the next line.
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy1, 0, 0);
		// With the above settings, draw the mesh.
		AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);


		AEVec2Set(&enemy1_a.center, enemy1_a.x, enemy1_a.y);
		// updates enemy position
		enemy1_a.x = enemy_update(enemy1_a.x);
	}
	// ------- XP for player -------
	else if (enemy1_a.Hp <= 0 && enemy1_a_Dead == FALSE) {
		player.XP += 10;
		enemy1_a_Dead = TRUE;
	}




	// ----------------  FOR NOW ONLY  ----------------
	// -------------- (TESTING PURPOSES) --------------

	if (enemy1_b.Hp > 0 && enemy1_b_Dead == FALSE) {
		// Tell the engine to get ready to draw something with texture.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can 
		// display the full range of colors (default is black).
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND
		// This will allow transparency.
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		//AEGfxSetTransparency(1.0f);
		// Set the texture
		AEGfxTextureSet(enemy1, 0, 0);
		// Create a scale matrix
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 60.f, 80.f);
		// Create a rotation matrix that rotates by 90 degrees
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);
		// Create a translation matrix that translates by
		// 100 in the x-axis and 100 in the y-axis
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, enemy1_b.x, enemy1_b.y);
		// Concatenate the matrices (TRS)
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to apply onto the vertices 
		// of the mesh that we are choose to draw in the next line.
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy1, 0, 0);
		// With the above settings, draw the mesh.
		AEGfxMeshDraw(enemy1_mesh, AE_GFX_MDM_TRIANGLES);


		// updates enemy position
		enemy1_b.x = enemy_update(enemy1_b.x);
	}
	// ------- XP for player -------
	else if (enemy1_b.Hp <= 0 && enemy1_b_Dead == FALSE) {
		player.XP += 10;
		enemy1_b_Dead = TRUE;
	}

	enemy_collision();
}

/*!**************************************************************************************************
\brief
	Calculates current enemy position based on total number of frames elapsed.

\param[in] EnemyCenter
	x coordinate of ememy

\return
	new enemy x position
*******************************************************************************************************/
f32 enemy_update (f32 enemy_x) {
	
	if (!isPaused) {
		// get 0-200
		s32 value = AEFrameRateControllerGetFrameCount() % 201;

		if (value <= 100)
			enemy_x -= 1.0f;
		else
			enemy_x += 1.0f;

		return enemy_x;
	}

	//AEGfxMeshFree(enemy_mesh);
	//AEGfxTextureUnload(enemy);
}

void enemy_collision(){
	if (damage_allowed == TRUE) {
		if (AETestRectToRect(&enemy1_a.center, ENEMY1_HEIGHT, ENEMY1_WIDTH, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			--player.Lives;
			damage_allowed = FALSE;
			//call transparancy function(?) to show invincibility
		}
	}
	else if(damage_allowed == FALSE) {
		if (AEFrameRateControllerGetFrameCount() % 100 == 0) {
			damage_allowed = TRUE;
			//set transparacny function(?) to false
		}

	}
}