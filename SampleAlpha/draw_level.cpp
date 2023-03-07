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

AEGfxTexture* rect, * trap;
AEGfxVertexList* rectmesh, * trapmesh;
Block leftright1, trapping, updown1;
extern Player_stats player;
//Player_stats* pointer_to_player{ &player };
extern Enemy1_stats enemy1;
bool damage_ok{ TRUE };

float moveSpeed = 150.f;


// NOTE: GRAVITY, BLOCK_WIDTH, BLOCK_HEIGHT defined in .hpp

void draw_level_init() {
	
	rect = AEGfxTextureLoad("Assets/grassMid.png");
	trap = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/spikes_top.png");
	rectmesh = leftright1.mesh = updown1.mesh = trapmesh = create_Square_Mesh();
	leftright1.x = 400.f;
	leftright1.y = 0.f;
	leftright1.max_x = 400.f + MID_DIST;
	leftright1.min_x = 250.f;
	updown1.x = 0.f;
	updown1.y = -100.f;
	updown1.max_y = -100.f;
	updown1.min_y = -200.f;

	//blocklist.emplace_back(Block());		//emplace_back constructs an object at the back of the vector.
	//blocklist[0].name = "move1";
	//blocklist[0].x = -200.f;
	//blocklist[0].y = -250.f;
	//blocklist[0].max_x = -200.f + 450.f;
	//blocklist[0].min_x = -200.f;

	//for (int i = 0; i < NUM_OF_MOVING_PLAT; i++) {

	//}
}

void draw_level() {

	// examples
	// 	enemy1.x = -300.0f;
	enemy1.y = -110.0f;
	//blocks(5, -500, -200);
	//blocks(4, -100, -50);
	//blocks(6, 200, 100);
	//blocks(4, 650, 0);
	//blocks(14, 1100, -200);
	//moving_blocks(5, move1.x, move1.y);
	//damanging_traps(2, -200, -200);
	blocks(8, -500, -200);
	updown_blocks(4, updown1.x, updown1.y);
	blocks(6, 0, 100);
	leftright_blocks(4, leftright1.x, leftright1.y);
	blocks(4, 1200, 200);
	spikes(1, 1425, 0);
	blocks(1, 1475, 0);
	spikes(1, 1525, 0);

	blocks(2, 1600, -100);
	spikes(16, 1700, -150);
	blocks(1, 2050, -50);
	blocks(2, 2450, -100);

	blocks(12, 2600, 0);
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
		platform_collision(length, x, y);
	}
}

int check_player_in_gravity_zone(Player_stats player) {
	//std::cout << "player x is" << player.x;
	if (player.x > static_cast<f32>(AEGetWindowWidth() / 2) && player.x < AEGetWindowWidth()) {
		return 1;
	}
	return 0;
}

void spikes(s32 length, f32 x, f32 y) {
	trapping.length = length;
	trapping.x = x;
	trapping.y = y;


	for (s32 i = 0; i < length; i++) {
		trapping.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		trapping.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, BLOCK_WIDTH, BLOCK_HEIGHT/2);
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x
			, BLOCK_HEIGHT / 4 *3 + y);
		// Concat the matrices
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);

		// Set the texture
		AEGfxTextureSet(trap, 0, 0);
		AEGfxMeshDraw(trapmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		AEVec2Set(&trapping.center, BLOCK_WIDTH / 2 + BLOCK_WIDTH * length + x , trapping.height);
		trap_collision(length, x, y);

	}
}

void leftright_blocks(s32 length, f32 x, f32 y) {

	leftright1.length = length;
	leftright1.x = x;
	leftright1.y = y;


	for (s32 i = 0; i < length; i++) {
		leftright1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		leftright1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&leftright1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&leftright1.rotate, PI);
		AEMtx33Trans(&leftright1.translate, leftright1.width, leftright1.height);
		// Concat the matrices
		AEMtx33Concat(&leftright1.transform, &leftright1.rotate, &leftright1.scale);
		AEMtx33Concat(&leftright1.transform, &leftright1.translate, &leftright1.transform);
		AEGfxSetTransform(leftright1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(leftright1.mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		
		platform_collision(length, x, y);


	}
	AEVec2Set(&leftright1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT);
}

void updown_blocks(s32 length, f32 x, f32 y) {

	updown1.length = length;
	updown1.x = x;
	updown1.y = y;


	for (s32 i = 0; i < length; i++) {
		updown1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		updown1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&updown1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&updown1.rotate, PI);
		AEMtx33Trans(&updown1.translate, updown1.width, updown1.height);
		// Concat the matrices
		AEMtx33Concat(&updown1.transform, &updown1.rotate, &updown1.scale);
		AEMtx33Concat(&updown1.transform, &updown1.translate, &updown1.transform);
		AEGfxSetTransform(updown1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(updown1.mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		platform_collision(length, x, y);


	}
	AEVec2Set(&updown1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT);
}

void move_update() {
	//in case of using sine
	f64* time = nullptr;
	//move1.x = 200 * sinf(static_cast<float>(AEGetTime(time)/0.5f));
	//if the x value is now == original position 

	if (leftright1.pos == OG) {
		leftright1.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&leftright1.center, BLOCK_WIDTH*4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (leftright1.x >= leftright1.max_x) {
			leftright1.pos = MOVED;
		}
	}

	if (leftright1.pos == MOVED) {
		leftright1.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&leftright1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (leftright1.x <= leftright1.min_x) {
			leftright1.pos = OG;
		}
	}

	if (updown1.pos == OG) {
		updown1.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&updown1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (updown1.y >= updown1.max_y) {
			updown1.pos = MOVED;
		}
	}

	if (updown1.pos == MOVED) {
		updown1.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&updown1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (updown1.y <= updown1.min_y) {
			updown1.pos = OG;
		}
	}
}


void platform_collision(s32 cnt, f32 x, f32 y) {
	for (f32 i = 0; i < cnt; i++) {
		if ((player.y <= BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2) &&
			(player.y >= y + PLAYER_HEIGHT / 2) &&
			(player.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * cnt + x - PLAYER_WIDTH / 2) &&
			(player.x >= x + BLOCK_WIDTH / 2 - PLAYER_WIDTH / 2))
			player.y = BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2;
	}
}

void trap_collision(s32 cnt, f32 x, f32 y) {
	
	if (((player.y <= BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2) &&
		(player.y >= y + PLAYER_HEIGHT / 2) &&
		(player.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * cnt + x - PLAYER_WIDTH / 2) &&
		(player.x >= x + BLOCK_WIDTH / 2 - PLAYER_WIDTH / 2)) == TRUE ) {

		player.y = BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2;
		if (damage_ok == TRUE) {
				--player.Hp;
				damage_ok = FALSE;
				//call transparancy function(?) to show invincibility
		}
		else if (damage_ok == FALSE) {
			if (AEFrameRateControllerGetFrameCount() % 75 == 0) {
				damage_ok = TRUE;
				//set transparacny function(?) to false
			}

		}
	}
	

	
}
