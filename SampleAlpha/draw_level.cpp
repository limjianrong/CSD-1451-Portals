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

#include "Player.hpp"
#include "Enemy.hpp"
#include <iostream>

int const platform_max_count{ 50 };

AEGfxTexture* rect, * trap;
AEGfxVertexList* rectmesh, * trapmesh,  *slippery_platform_mesh{};
Slippery_platform slippery_platform[platform_max_count];
Block leftright1, trapping, updown1, diagup1, diagdown1, oneuse1, verti1;
extern Player_stats player;
//Player_stats* pointer_to_player{ &player };
//extern Enemy1_stats enemy1;
bool damage_ok{ TRUE };

float moveSpeed = 150.f;

AEMtx33 scale, rotate, translate, transform; // TRS

// NOTE: GRAVITY, BLOCK_WIDTH, BLOCK_HEIGHT defined in .hpp

void draw_level_load() {
	rect = AEGfxTextureLoad("Assets/grassMid.png");
	trap = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/spikes_top.png");
	//rectmesh = leftright1.mesh = updown1.mesh = diagup1.mesh = diagdown1.mesh = oneuse1.mesh = verti1.mesh = trapmesh = create_Square_Mesh();
	rectmesh = create_Square_Mesh();
	//rectmesh = leftright1.mesh = updown1.mesh = trapmesh = create_Square_Mesh();
	slippery_platform_mesh = create_Square_Mesh();
}

void draw_level_init() {
	
	leftright1.x = 400.f;
	leftright1.y = 0.f;
	leftright1.end_x = 400.f + MID_DIST;
	leftright1.start_x = 250.f;
	updown1.x = 0.f;
	updown1.y = -100.f;
	updown1.end_y = -100.f;
	updown1.min_y = -200.f;

	diagup1.x = 400.f;
	diagup1.y = 100.f;
	diagup1.end_x = 600.f;
	diagup1.start_x = 100.f;
	diagup1.end_y = 250.f;
	diagup1.min_y = 100.f;

	//diagdown1.x = -250.f;
	//diagdown1.y = 250.f;
	//diagdown1.end_x = -150.f;
	//diagdown1.start_x = -250.f;
	//diagdown1.end_y = 150.f;
	//diagdown1.min_y = 250.f;

	oneuse1.x = -400.f;
	oneuse1.y = 150.f;
	oneuse1.flag = ACTIVE;

	verti1.x = -300.f;
	verti1.y = -150.f;

	//initialize a single slippery platform
	slippery_platform[0].status = true;
	slippery_platform[0].x = 0;
	slippery_platform[0].y = 0;

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
	//enemy1.y = -110.0f;
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
	diag_up_blocks(4, diagup1.x, diagup1.y);
	//diag_down_blocks(4, diagdown1.x, diagdown1.y);
	one_use_blocks(4, oneuse1.x, oneuse1.y);
	//verti_blocks(3, verti1.x, verti1.y);
	blocks(4, 1200, 200);
	spikes(1, 1425, 0);
	blocks(1, 1475, 0);
	spikes(1, 1525, 0);

	blocks(2, 1600, -100);
	spikes(16, 1700, -150);
	blocks(1, 2050, -50);
	blocks(2, 2450, -100);

	blocks(12, 2600, 0);
	draw_slippery_platform();
}

void update_level() {
	// Creates an anti-gravity zone
	anti_gravity_zone(1000, 1200);
	update_slippery_platform();
}

//-------------------- Parameters --------------------
// length -> how many blocks to draw
// x -> x coordinates of starting block (-500 to 500)
// y -> y coordinates of starting block (-300 to 300)
// ---------------------------------------------------
void blocks(s32 length, f32 x, f32 y) {

	for (s32 i = 0; i < length; i++) {
		AEMtx33Scale(&scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&rotate, PI);
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
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

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
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		
		platform_collision(length, x, y);


	}
	AEVec2Set(&leftright1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT/4*3);
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
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		platform_collision(length, x, y);


	}
	AEVec2Set(&updown1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT / 4 * 3);
}

void diag_up_blocks(s32 length, f32 x, f32 y) {

	diagup1.length = length;
	diagup1.x = x;
	diagup1.y = y;


	for (s32 i = 0; i < length; i++) {
		diagup1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		diagup1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&diagup1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&diagup1.rotate, PI);
		AEMtx33Trans(&diagup1.translate, diagup1.width, diagup1.height);
		// Concat the matrices
		AEMtx33Concat(&diagup1.transform, &diagup1.rotate, &diagup1.scale);
		AEMtx33Concat(&diagup1.transform, &diagup1.translate, &diagup1.transform);
		AEGfxSetTransform(diagup1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		platform_collision(length, x, y);


	}
	AEVec2Set(&diagup1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT / 4 * 3);
}

void diag_down_blocks(s32 length, f32 x, f32 y) {

	diagdown1.length = length;
	diagdown1.x = x;
	diagdown1.y = y;


	for (s32 i = 0; i < length; i++) {
		diagdown1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		diagdown1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&diagdown1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&diagdown1.rotate, PI);
		AEMtx33Trans(&diagdown1.translate, diagdown1.width, diagdown1.height);
		// Concat the matrices
		AEMtx33Concat(&diagdown1.transform, &diagdown1.rotate, &diagdown1.scale);
		AEMtx33Concat(&diagdown1.transform, &diagdown1.translate, &diagdown1.transform);
		AEGfxSetTransform(diagdown1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		platform_collision(length, x, y);


	}
	AEVec2Set(&diagdown1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT / 4 * 3);
}

void one_use_blocks(s32 length, f32 x, f32 y) {

	oneuse1.length = length;
	oneuse1.x = x;
	oneuse1.y = y;


	for (s32 i = 0; i < length; i++) {
		oneuse1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		oneuse1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&oneuse1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&oneuse1.rotate, PI);
		AEMtx33Trans(&oneuse1.translate, oneuse1.width, oneuse1.height);
		// Concat the matrices
		AEMtx33Concat(&oneuse1.transform, &oneuse1.rotate, &oneuse1.scale);
		AEMtx33Concat(&oneuse1.transform, &oneuse1.translate, &oneuse1.transform);
		AEGfxSetTransform(oneuse1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		if (oneuse1.flag == ACTIVE) {
			AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);
			// Player collision with platforms
			platform_collision(length, x, y);
		}
		
	}
	AEVec2Set(&oneuse1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT / 4 * 3);
}


void verti_blocks(s32 length, f32 x, f32 y) {

	verti1.length = length;
	verti1.x = x;
	verti1.y = y;


	for (s32 i = 0; i < length; i++) {
		diagdown1.width = BLOCK_WIDTH / 2 + x;
		diagdown1.height = BLOCK_HEIGHT / 2 + BLOCK_WIDTH * i + y;

		AEMtx33Scale(&diagdown1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&diagdown1.rotate, PI);
		AEMtx33Trans(&diagdown1.translate, diagdown1.width, diagdown1.height);
		// Concat the matrices
		AEMtx33Concat(&diagdown1.transform, &diagdown1.rotate, &diagdown1.scale);
		AEMtx33Concat(&diagdown1.transform, &diagdown1.translate, &diagdown1.transform);
		AEGfxSetTransform(diagdown1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(rectmesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		verti_collision(length, x, y);


	}
	//AEVec2Set(&diagdown1.center, (BLOCK_WIDTH * length) / 2 + x, y + BLOCK_HEIGHT);
}
void move_update() {
	//in case of using sine
	f64* time = nullptr;
	//move1.x = 200 * sinf(static_cast<float>(AEGetTime(time)/0.5f));
	//if the x value is now == original position 

	if (leftright1.pos == OG) {
		leftright1.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&leftright1.center, BLOCK_WIDTH*4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (leftright1.x >= leftright1.end_x) {
			leftright1.pos = MOVED;
		}
	}

	if (leftright1.pos == MOVED) {
		leftright1.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&leftright1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (leftright1.x <= leftright1.start_x) {
			leftright1.pos = OG;
		}
	}

	if (updown1.pos == OG) {
		updown1.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&updown1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (updown1.y >= updown1.end_y) {
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

	if (diagup1.pos == OG) {
		diagup1.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
		diagup1.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&diagup1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			player.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
			player.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		}
		if (diagup1.y >= diagup1.end_y ) { //&& diagup1.x >= diagup1.end_x
			diagup1.pos = MOVED;
		}
	}

	if (diagup1.pos == MOVED) {
		diagup1.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		diagup1.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&diagup1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			player.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
			player.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		}
		if (diagup1.y <= diagup1.min_y) {
			diagup1.pos = OG;
		}
	}

	if (diagdown1.pos == OG) {
		diagdown1.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
		diagdown1.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&diagdown1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			player.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
			player.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		}
		if (diagdown1.y <= diagdown1.end_y ) {//&& diagdown1.x >= diagdown1.end_x
			diagdown1.pos = MOVED;
		}
	}

	if (diagdown1.pos == MOVED) {
		diagdown1.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
		diagdown1.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		if (AETestRectToRect(&diagdown1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			player.x -= AEFrameRateControllerGetFrameTime() * moveSpeed;
			player.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
		}
		if (diagdown1.y >= diagdown1.min_y) {
			diagdown1.pos = OG;
		}
	}


	if (AETestRectToRect(&oneuse1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
		oneuse1.timer += AEFrameRateControllerGetFrameTime();
		if (oneuse1.timer >= 5) oneuse1.flag = NOT_ACTIVE; 
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

void verti_collision(s32 cnt, f32 x, f32 y) {
	for (f32 i = 0; i < cnt; i++) {
		if ((player.y <= BLOCK_HEIGHT + y + PLAYER_HEIGHT / 2) &&
			(player.y >= y + PLAYER_HEIGHT / 2) &&
			(player.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * cnt + x - PLAYER_WIDTH / 2) &&
			(player.x >= x + BLOCK_WIDTH / 2 - PLAYER_WIDTH / 2))
			player.x = -BLOCK_WIDTH + x + PLAYER_WIDTH / 2;
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

// x1 - x2 will be the anti-gravity zone
void anti_gravity_zone(f64 x1, f64 x2) {
	
	if (player.x >= x1 && player.x <= x2)
		player.y += GRAVITY;
	else // player <= x2
		player.y -= GRAVITY;

}

void draw_anti_gravity_zone(f64 x1, f64 x2) {

	/*AEGfxSetTransparency(0.55f);
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/
}


void draw_slippery_platform() {
	for (int i = 0; i < platform_max_count; ++i) {
		if (slippery_platform[i].status == false) {
			continue;
		}

		AEGfxSetTransform(slippery_platform[i].transform.m);
		AEGfxMeshDraw(slippery_platform_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void update_slippery_platform() {
	AEMtx33 scale{}, translate{};
	for (int i = 0; i < platform_max_count; ++i) {
		if (slippery_platform[i].status == false) {
			continue;
		}
		AEVec2Set(&slippery_platform[i].center, slippery_platform[i].x, slippery_platform[i].y);
		AEMtx33Scale(&scale, slippery_platform[i].width, slippery_platform[i].height);
		AEMtx33Trans(&translate, slippery_platform[i].x, slippery_platform[i].y);
		AEMtx33Concat(&slippery_platform[i].transform, &rotate, &scale);
		if (AECalcDistPointToRect(&player.bottom_hotspot, &slippery_platform[i].center, slippery_platform[i].width, slippery_platform[i].height) <= 0.0f) {
			player.falling == true;
		}

	}

}