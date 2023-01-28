#include "AEEngine.h"
#include <AEGraphics.h>
#include <ctime>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Utilities.hpp"

AEGfxTexture* enemy;
AEGfxVertexList* enemy_mesh;
AEVec2 EnemyCenter;
extern s32 enemy_HP = 5; // 1 bullet decrement by 1


void enemy_init() {

	enemy = AEGfxTextureLoad("Assets/enemy.png");
	// Saving the mesh (list of triangles) in enemy_mesh
	enemy_mesh = create_Square_Mesh();
	
	// Starting x & y value of enemy
	AEVec2Set(&EnemyCenter, -250.0f, -85.0f);
}

void draw_enemy() {

	if (enemy_HP > 0) {
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
		AEMtx33Trans(&translate, EnemyCenter.x, EnemyCenter.y);
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
		EnemyCenter = enemy_update(EnemyCenter);
	}

	//enemy_x = rand() % WINDOWXLENGTH + WINDOWXLENGTH;
	//enemy_y = rand() % WINDOWYLENGTH;

	
}

AEVec2 enemy_update (AEVec2 EnemyCenter) {
	
	// get 0-200
	s32 value = AEFrameRateControllerGetFrameCount() % 201;

	if (value <= 100)
		EnemyCenter.x -= 1.0f;
	else
		EnemyCenter.x += 1.0f;	

	return EnemyCenter;

	//AEGfxMeshFree(enemy_mesh);
	//AEGfxTextureUnload(enemy);
}