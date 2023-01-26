#include "AEEngine.h"
#include <AEGraphics.h>
#include <ctime>
#include "Player.hpp"

#define WINDOWXLENGTH 1000
#define WINDOWYLENGTH 600

AEGfxTexture* enemy;
AEGfxVertexList* enemy_mesh;
f32 enemy_x{}, enemy_y{};

void enemy_init() {

	enemy = AEGfxTextureLoad("Assets/enemy.png");

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	enemy_mesh = AEGfxMeshEnd();

}

void draw_enemy() {

	// Set the background to black.
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	// Tell the engine to get ready to draw something with texture.
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can 
	// display the full range of colors (default is black).
	//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxSetTransparency(1.0f);
	// Set the texture to pTex
	AEGfxTextureSet(enemy, 0, 0);
	// Create a scale matrix that scales by 100 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 100.f, 100.f);
	// Create a rotation matrix that rotates by 45 degrees
	AEMtx33 rotate = { 0 };
	//AEMtx33Rot(&rotate, PI / 4);
	// Create a translation matrix that translates by
	// 100 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 100.f, 100.f);
	// Concatenate the matrices (TRS)
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to apply onto the vertices 
	// of the mesh that we are choose to draw in the next line.
	AEGfxSetTransform(transform.m);
	// With the above settings, draw the mesh.
	AEGfxMeshDraw(enemy_mesh, AE_GFX_MDM_TRIANGLES);

	AEVec2 EnemyCenter{};

	enemy_x = rand() % WINDOWXLENGTH + WINDOWXLENGTH;
	enemy_y = rand() % WINDOWYLENGTH;

	AEVec2Set(&EnemyCenter, 0, 0);
	
}

void enemy_update() {
	//enemy_x -= (enemy_x - playerx) * 0.1;
	//enemy_y -= (enemy_y - playery) * 0.1;

	//AEGfxMeshFree(enemy_mesh);
	//AEGfxTextureUnload(enemy);
}