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
#include <iostream>
using namespace std;

AEGfxTexture* rect, *trap;
AEGfxVertexList* rectmesh, *trapmesh;

extern Player_stats player;
//Player_stats* pointer_to_player{ &player };
extern Enemy_stats enemy1;

move_block move1;
// NOTE: GRAVITY, BLOCK_WIDTH, BLOCK_HEIGHT defined in .hpp

void draw_level_init() {

	rect = AEGfxTextureLoad("Assets/simplified_png/PNG/Tiles/platformPack_tile001.png");
	trap = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/spikes_top.png");
	rectmesh = move1.mesh = trapmesh = create_Square_Mesh();

	for (int i = 0; i < NUM_OF_MOVING_PLAT; i++) {

	}
}

void draw_level() {

	// examples
	blocks(5, -500, -200);
	blocks(4, -100, -50);
	blocks(6, 200, 100);
	blocks(4, 650, 0);
	blocks(14, 1100, -200);
	moving_blocks(5, -200, -250);
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

void traps(s32 length, f32 x, f32 y) {

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
		AEGfxTextureSet(trap, 0, 0);
		AEGfxMeshDraw(trapmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		if ((player.y <= BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2) &&
			(player.y >= y + PLAYER_HEIGHT / 2) &&
			(player.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * length + x - PLAYER_WIDTH / 2) &&
			(player.x >= x + BLOCK_WIDTH / 2 - PLAYER_WIDTH / 2))
			player.y = BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2 + 5;
	}
}


void moving_blocks(s32 length, f32 x, f32 y) {

	move1.length = length;
	move1.x = x;
	move1.y = y;
	

	for (s32 i = 0; i < length; i++) {
		move1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		move1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&move1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&move1.rotate, PI);
		AEMtx33Trans(&move1.translate, move1.width, move1.height);
		// Concat the matrices
		AEMtx33Concat(&move1.transform, &move1.rotate, &move1.scale);
		AEMtx33Concat(&move1.transform, &move1.translate, &move1.transform);
		AEGfxSetTransform(move1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(move1.mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		if ((player.y <= BLOCK_HEIGHT + move1.y + PLAYER_HEIGHT / 2) &&
			(player.y >= move1.y + PLAYER_HEIGHT / 2) &&
			(player.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * length + move1.x - PLAYER_WIDTH / 2) &&
			(player.x >= move1.x + BLOCK_WIDTH / 2 - PLAYER_WIDTH / 2))
			player.y = BLOCK_HEIGHT + move1.y + PLAYER_HEIGHT / 2;


		
	}
	//move1.x = move_update(move1.x);
	//move_update();
	
}

void move_update() {
	f32 og_pos = move1.width;
	f32 dist = og_pos + 100;
	cout << "OG position:      " << og_pos << endl;
	cout << "MOVED position:      " << dist << endl;

	if (move1.pos == OG) {
		while (move1.pos == OG) {
			move1.width += 10.f;
			if (move1.width >= dist) {
				move1.pos = MOVED;
				break;
			}
		}
	}

	cout << "AFTER MOVING:      " << move1.width << endl << endl;

	if (move1.pos == MOVED) {
		while (move1.pos == MOVED){
			move1.width -= 10.f;
			if (move1.width <= og_pos) {
				move1.pos = OG;
				break;
			}
		}
		
	}

	cout << "AFTER RETURNING:      " << move1.width << endl << endl;
}

//f32 move_update(f32 x) {
//
//	f32 og_pos = x;
//	f32 dist = og_pos + 100;
//
//	if (move1.pos == OG) {
//		x += 10.f;
//		if (x >= dist) move1.pos = MOVED;
//	}
//
//	if (move1.pos == MOVED) {
//		x -= 10.f;
//		if (x <= og_pos) move1.pos = OG;
//	}
//
//	return x;
//
//	cout << "testing:      " << move1.x << endl << endl;
//
//}
