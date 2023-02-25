#include "AEEngine.h"
#include <AEGraphics.h>
#include "Player.hpp"
#include "Utilities.hpp"
#include "Enemy3.hpp"





// ----- Mesh & Texture -----
AEGfxTexture* enemy3;
AEGfxVertexList* enemy3_mesh;

AEGfxTexture* enemy3_warning;
AEGfxVertexList* enemy3_warning_mesh;

// ----- Enemy -----
Enemy3_stats enemy3_a;
bool enemy3_a_Dead;

// ----- Player -----
extern Player_stats player;

// ----- Pause Menu -----
extern bool isPaused;

/*!**************************************************************************************************
\brief
	Loads texture and initializes mesh for enemy
*******************************************************************************************************/


void enemy3_init() {

	enemy3 = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/wingMan3.png");
	// Saving the mesh (list of triangles) in enemy3_mesh
	enemy3_mesh = create_Square_Mesh();

	enemy3_warning = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/powerup_wings.png");
	// Saving the mesh (list of triangles) in enemy3_mesh
	enemy3_warning_mesh = create_Square_Mesh();

	// spawn outside screen
	enemy3_a.x = WINDOWLENGTH_X/2 + 500.f;
	enemy3_a.y = 0;
	
}

void draw_enemy3() {

	if (enemy3_a.Hp > 0 && enemy3_a_Dead == FALSE) {
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
		AEGfxTextureSet(enemy3, 0, 0);
		// Create a scale matrix
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, ENEMY3_WIDTH, ENEMY3_HEIGHT);
		// Create a rotation matrix that rotates by 90 degrees
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);
		// Create a translation matrix that translates by
		// 100 in the x-axis and 100 in the y-axis
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, enemy3_a.x, enemy3_a.y);
		// Concatenate the matrices (TRS)
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to apply onto the vertices 
		// of the mesh that we are choose to draw in the next line.
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(enemy3, 0, 0);
		// With the above settings, draw the mesh.
		AEGfxMeshDraw(enemy3_mesh, AE_GFX_MDM_TRIANGLES);

		if (enemy3_a.x > (WINDOWLENGTH_X/2 + ENEMY3_WIDTH/2)) {
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
			AEGfxTextureSet(enemy3_warning, 0, 0);
			// Create a scale matrix
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			// Create a rotation matrix that rotates by 90 degrees
			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, PI);
			// Create a translation matrix that translates by
			// 100 in the x-axis and 100 in the y-axis
			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, WINDOWLENGTH_X/2 - WARNING_WIDTH/2, enemy3_a.y);
			// Concatenate the matrices (TRS)
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			// Choose the transform to apply onto the vertices 
			// of the mesh that we are choose to draw in the next line.
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			// With the above settings, draw the mesh.
			AEGfxMeshDraw(enemy3_warning_mesh, AE_GFX_MDM_TRIANGLES);
		}
		else if (enemy3_a.x < ((-WINDOWLENGTH_X / 2) - ENEMY3_WIDTH)) {
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
			AEGfxTextureSet(enemy3_warning, 0, 0);
			// Create a scale matrix
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, WARNING_WIDTH, WARNING_HEIGHT);
			// Create a rotation matrix that rotates by 90 degrees
			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, PI);
			// Create a translation matrix that translates by
			// 100 in the x-axis and 100 in the y-axis
			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, ( - WINDOWLENGTH_X / 2) + WARNING_WIDTH/2, enemy3_a.y);
			// Concatenate the matrices (TRS)
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			// Choose the transform to apply onto the vertices 
			// of the mesh that we are choose to draw in the next line.
			AEGfxSetTransform(transform.m);
			AEGfxTextureSet(enemy3_warning, 0, 0);
			// With the above settings, draw the mesh.
			AEGfxMeshDraw(enemy3_warning_mesh, AE_GFX_MDM_TRIANGLES);
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

void enemy3_update(Player_stats* player) {
	Enemy3_stats* Enemy3_a = &enemy3_a;

	if (!isPaused) {
		// get 0-200
		s32 value = AEFrameRateControllerGetFrameCount() % 201;

		if (value <= 100) {
			Enemy3_a->y = player->y;
			Enemy3_a->x -= 10.f;

		}
		else {
			Enemy3_a->y = player->y;
			Enemy3_a->x -= 1.f;
		}
	}
}

void enemy3_collision(Player_stats* player) {}

