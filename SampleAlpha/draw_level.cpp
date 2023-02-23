/******************************************************************************//*!
\file		draw_level.cpp
\author 	Digipen, Tay Zhun Hang
\par    	email: zhunhang.tay@digipen.edu
\date   	12 February, 2023
\brief		

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#include "AEEngine.h"
#include "draw_level.hpp"
#include "Utilities.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

AEGfxTexture* rect;
AEGfxVertexList* rectmesh;

extern Player_stats player;
//Player_stats* pointer_to_player{ &player };
extern Enemy_stats enemy1;

// NOTE: GRAVITY, BLOCK_WIDTH, BLOCK_HEIGHT defined in .hpp

void draw_level_init() {

	rect = AEGfxTextureLoad("Assets/simplified_png/PNG/Tiles/platformPack_tile001.png");

	rectmesh = create_Square_Mesh();

}

void draw_level() {

	// examples
	blocks(5, -500, -200);
	blocks(4, -100, -50);
	blocks(6, 200, 100);
	blocks(4, 650, 0);
	blocks(14, 1100, -200);

	//AEGfxMeshFree(rectmesh);
	//AEGfxTextureUnload(rect);

}

void update_level() {
	player.y -= GRAVITY;
	//if (
	//	check_player_in_gravity_zone(player)) {
	//	player.y += GRAVITY;
	//}
	//else {
	//	player.y -= GRAVITY;
	//	//pointer_to_player->y -= GRAVITY;
	//}


}

//-------------------- Parameters --------------------
// length -> how many blocks to draw
// x -> x coordinates of starting block (-500 to 500)
// y -> y coordinates of starting block (-300 to 300)
// ---------------------------------------------------
void blocks(s32 length, f32 x, f32 y) {

	for (s32 i = 0; i < length; i++) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x
							   , BLOCK_HEIGHT / 2 + y);
		// Concat the matrices
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		if ((player.y <= BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2) &&
			(player.y >= y + PLAYER_HEIGHT / 2) &&
			(player.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * length + x - PLAYER_WIDTH/2) &&
			(player.x >= x + BLOCK_WIDTH / 2 - PLAYER_WIDTH / 2))
			player.y = BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2 + 5;
	}
}

int check_player_in_gravity_zone(Player_stats player) {
	//std::cout << "player x is" << player.x;
	if (player.x > static_cast<f32>(AEGetWindowWidth()/2) && player.x < AEGetWindowWidth()) {
		return 1;
	}
	return 0;
}
