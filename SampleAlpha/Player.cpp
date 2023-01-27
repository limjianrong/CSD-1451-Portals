#include "AEEngine.h"

#include "Player.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "Utilities.hpp"

int playersize{ 200 };



f32 rotation{ 0 };
f32 playerx{  }, playery{};
AEGfxVertexList* pMesh;
AEGfxVertexList* trianglemesh;


AEGfxTexture* pTex;
void initialize_player(int playersize) { //PLAYERSIZE is not used for now

	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();
	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures 0.0 - 1.0
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	
}

void draw_player(int playersize) {
	AEVec2 PlayerCenter{};

	// Create a scale matrix that scales by 100 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 50.f, 50.f);

	// Create a rotation matrix that rotates by rotation degrees
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, playerx, playery);
	// Concat the matrices (TRS)
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to use
	AEGfxSetTransform(transform.m);

	// Set the texture to pTex
	AEGfxTextureSet(pTex, 0, 0);
	
	// Actually drawing the mesh 
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	AEVec2Set(&PlayerCenter, playerx, playery);
	// Call player movement function so x & y values can be translated (to be able to move)
	player_movement(PlayerCenter);
	portal_feature(&PlayerCenter, playerx,playery);
	draw_a_portal(&PlayerCenter,playerx,playery);
	/*AEMtx33 scale2 {};
	AEMtx33Scale(&scale2, 100.0f, 100.0f);
	AEMtx33 translate2{};
	AEMtx33 finalform{};
	AEMtx33Trans(&translate2, 100.0f, 100.0f);
	AEMtx33Concat(&finalform, &scale2, &translate2);
	AEGfxSetTransform(finalform.m);
	
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/

	//bullet_mesh();
	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(playerx, playery, 1);
	}
	else {
		weapon_fire(playerx, playery, 0);
	}




}

void player_movement(AEVec2 PlayerCenter) {
	// A key pressed
	if (AEInputCheckCurr(AEVK_A)) {
		playerx -= 5;
		rotation += 0.1f;
	}
	// D key pressed
	else if (AEInputCheckCurr(AEVK_D)) {
		playerx += 5;
		rotation -= 0.1f;
	}
	// W key pressed (No rotation)
	if (AEInputCheckPrev(AEVK_W) && AEInputCheckCurr(AEVK_W)) playery += 5;
	// S key pressed (No rotation)
	else if (AEInputCheckPrev(AEVK_S) && AEInputCheckCurr(AEVK_S)) playery -= 5;


	// player to never go out of frame -zy

	if (playerx < ( - WINDOWXLENGTH / 2) + 25)
		playerx = (- WINDOWXLENGTH / 2) + 25;

	if (playerx > WINDOWXLENGTH/2 - 25)
		playerx = WINDOWXLENGTH/2 - 25;

	if (playery < ( - WINDOWYLENGTH / 2) + 25)
		playery = ( - WINDOWYLENGTH / 2) + 25;

	if (playery > WINDOWYLENGTH/2 - 25)
		playery = WINDOWYLENGTH/2 - 25;


	// rect 300 by 75, (-500,-200) (-150,-100) (175, 50)
	// playery = y coordinate of rectangle + height of rectangle mesh +
	// 1/2 height of player mesh (since player x,y is located in player center) -zy

	// playerx along lowest platform
	if (playerx >= -500 && playerx <= -200)
		playery = -200 + 75 + 25;

	// playerx along middle platform
	if (playerx >= -150 && playerx <= 150)
		playery = -100 + 75 + 25;

	// playerx along top platform
	if (playerx >= 175 && playerx <= 475)
		playery = 50 + 75 + 25;

 	
}


