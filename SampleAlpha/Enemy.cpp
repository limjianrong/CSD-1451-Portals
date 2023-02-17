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

AEGfxTexture* enemy;
AEGfxVertexList* enemy_mesh;

Enemy_stats enemy1;

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/
void enemy_init() {

	enemy = AEGfxTextureLoad("Assets/enemy.png");
	// Saving the mesh (list of triangles) in enemy_mesh
	enemy_mesh = create_Square_Mesh();

}

/*!**************************************************************************************************
\brief
	Draws enemy if enemy HP is more than 0 to its updated position from function enemy_update
*******************************************************************************************************/
void draw_enemy() {

	if (enemy1.Hp > 0) {
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
		AEGfxTextureSet(enemy, 0, 0);
		// Create a scale matrix
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 60.f, 80.f);
		// Create a rotation matrix that rotates by 90 degrees
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);
		// Create a translation matrix that translates by
		// 100 in the x-axis and 100 in the y-axis
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, enemy1.x, enemy1.y);
		// Concatenate the matrices (TRS)
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to apply onto the vertices 
		// of the mesh that we are choose to draw in the next line.
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy, 0, 0);
		// With the above settings, draw the mesh.
		AEGfxMeshDraw(enemy_mesh, AE_GFX_MDM_TRIANGLES);


		// updates enemy position
		enemy1.x = enemy_update(enemy1.x);
	}


	
}

/*!**************************************************************************************************
\brief
	Calculates current enemy position based on total number of frames elapsed.

\param[in] EnemyCenter
	x and y coordinate of ememy

\return
	new EnemyCenter
*******************************************************************************************************/
f32 enemy_update (f32 enemy1_x) {
	
	// get 0-200
	s32 value = AEFrameRateControllerGetFrameCount() % 201;

	if (value <= 100)
		enemy1_x -= 1.0f;
	else
		enemy1_x += 1.0f;

	return enemy1_x;

	//AEGfxMeshFree(enemy_mesh);
	//AEGfxTextureUnload(enemy);
}