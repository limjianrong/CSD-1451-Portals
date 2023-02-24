#include "AEEngine.h"
#include <AEGraphics.h>
#include "Player.hpp"
#include "Utilities.hpp"
#include "Enemy3.hpp"

/*



// ----- Mesh & Texture -----
AEGfxTexture* enemy3;
AEGfxVertexList* enemy3_mesh;

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

/*
void enemy3_init() {
	enemy3 = AEGfxTextureLoad("\Assets\jumperpack\PNG\Enemies\spikeMan_stand.png");
	// Saving the mesh (list of triangles) in enemy_mesh
	enemy3_mesh = create_Square_Mesh();

	// random
	enemy3_a.x = -100.f;
	enemy3_a.y = -100.f;
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
		AEMtx33Scale(&scale, 60.f, 80.f);
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


		// updates enemy position
		// enemy3_a.x = enemy3_update(enemy3_a.x);
	}
	// ------- XP for player -------
	else if (enemy3_a.Hp <= 0 && enemy3_a_Dead == FALSE) {
		player.XP += 30;
		enemy3_a_Dead = TRUE;
	}
}

//f32 enemy3_update(f32 enemy_x) {}

//void enemy3_collision(Player_stats player) {}

*/