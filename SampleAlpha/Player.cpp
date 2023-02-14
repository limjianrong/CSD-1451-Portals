/******************************************************************************//*!
\file		Player.cpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	1 February, 2023
\brief		Source file for the platformer game state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#include "AEEngine.h"

#include "Player.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "Utilities.hpp"
#include <iostream> //for std::cout
AEGfxVertexList* pMesh;
AEGfxVertexList* trianglemesh;
AEGfxVertexList* endpoint_triangle;
AEGfxVertexList* endpoint_rectangle;
AEVec2 endpoint_center;


Player_stats player;

AEGfxTexture* pTex;
f32 cameraX{}, cameraY{};
void initialize_player() { 

	pTex = AEGfxTextureLoad("Assets/simplified_png/PNG/Tiles/platformPack_tile024.png");
	pMesh = create_Square_Mesh();

	bullet_initialise();
}

void draw_player() {
	// Create a scale matrix that scales by 100 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, PLAYER_WIDTH, PLAYER_HEIGHT);
	// Create a rotation matrix that rotates by rotation degrees
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, player.rotation);
	// Translate
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, player.x, player.y);
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
	
	//check_endpoint(player.x, player.y, endpoint_rectangle,endpoint_triangle, &PlayerCenter, endpoint_center);

}

void update_player() {

	// ---------  Player's movement   -----------
	// A key pressed
	if (AEInputCheckCurr(AEVK_A)) {
		player.x -= 5;
		player.rotation += 0.1f;
		//AEGfxGetCamPosition(&cameraX, &cameraY);
		
	}
	// D key pressed
	else if (AEInputCheckCurr(AEVK_D)) {
		player.x += 5;
		player.rotation -= 0.1f;

	}
	if (player.x > 0) {
		AEGfxSetCamPosition(player.x, 0);
	}

	// ---------  Firing of bullets   -----------
	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		weapon_fire(player.x, player.y, 1);
	}
	else {
		weapon_fire(player.x, player.y, 0);
	}

	// ---------  Portal creation   -----------
	draw_portal(player.x, player.y);

	// ------------  Collision   --------------
	player_collision();
}



void player_collision() {


	// player to never go out of frame -zy
	if (player.x < (-WINDOWXLENGTH / 2) + PLAYER_WIDTH / 2)
		player.x = (-WINDOWXLENGTH / 2) + PLAYER_WIDTH / 2;



	// bottom of screen
	if (player.y < (-WINDOWYLENGTH / 2) + PLAYER_HEIGHT / 2)
		player.y = (-WINDOWYLENGTH / 2) + PLAYER_HEIGHT / 2;

	//if (player.y > WINDOWYLENGTH / 2 - 25)
	//	player.y = WINDOWYLENGTH/2 - 25;
	
}

void endpoint_init() {
	//draw a triangle to represent the flag(endpoint)
	AEGfxMeshStart();
	AEGfxTriAdd(-50.0f, 0.0f, 0xFFFFFF00, 0.0, 0.0,
		0.0f, 25.0f, 0xFFFFFF00, 0.0f, 0.0f,
		0.0f, -25.0f, 0xFFFFFF00, 0.0f, 0.0f);
	
	endpoint_triangle = AEGfxMeshEnd();

	//draw a rectangle to represent the flagpole
	AEGfxMeshStart();
	AEGfxVertexAdd(0.0f, 25.0f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxVertexAdd(10.0f, 25.0f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxVertexAdd(10.0f, -75.0f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxVertexAdd(0.0f, -75.0f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxVertexAdd(0.0f, 25.0f, 0xFFFFFF00, 0.0f, 0.0f);
	AEGfxVertexAdd(10.0f, -75.0f, 0xFFFFFF00, 0.0f, 0.0f);
	endpoint_rectangle = AEGfxMeshEnd();

	AEVec2Set(&endpoint_center, 400.0f+(0.0f + 10.0f) / 2, 200+(25.0f-75.0f/2));

}

void check_endpoint(f32 playerx, f32 playery, AEGfxVertexList* endpoint_rectangle, 
	AEGfxVertexList*endpoint_triangle, AEVec2* PlayerCenter, AEVec2& endpoint_center) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetPosition(endpoint_center.x, endpoint_center.y);
	AEGfxMeshDraw(endpoint_triangle, AE_GFX_MDM_TRIANGLES);
	AEGfxMeshDraw(endpoint_rectangle, AE_GFX_MDM_TRIANGLES);
	if (AETestRectToRect(PlayerCenter, PLAYER_WIDTH, PLAYER_HEIGHT, &endpoint_center, 10.0f,100.0f)) {
	}
}
