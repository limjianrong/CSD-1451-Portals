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

AEGfxTexture* rect, * upwardspike, *downwardspike;

extern AEGfxVertexList* square_mesh;	// Created square mesh



AEGfxVertexList* rectmesh, * trapmesh;

Block leftright1, trapping, updown1, diagup1, diagdown1, oneuse1, verti1, droptrap1, horizontaltrap1;
Block normal[MAX_NORMAL], leftright[MAX_LEFT_RIGHT], updown[MAX_UP_DOWN], diagonalup[MAX_DIAGONAL_UP], 
diagonaldown[MAX_DIAGONAL_DOWN], onetimeuse[MAX_ONE_TIME_USE], verticalwall[MAX_VERTICAL_WALL];
extern Player_stats player;
extern Enemy1_stats enemy1[MAX_ENEMIES_1];
bool damage_ok{ TRUE };

float moveSpeed = 150.f;
float dropSpeed = 400.f;

AEMtx33 scale, rotate, translate, transform; // TRS

// NOTE: GRAVITY, BLOCK_WIDTH, BLOCK_HEIGHT defined in .hpp

void draw_level_load() {
	rect = AEGfxTextureLoad("Assets/grassMid.png");
	upwardspike = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/spikes_top.png");
	downwardspike = AEGfxTextureLoad("Assets/jumperpack/PNG/Environment/spikes_bottom.png");

	//for (s32 i = 0; i < MAX_LEFT_RIGHT; ++i) {
	//	leftright[i].texture = AEGfxTextureLoad("Assets/grassMid.png");
	//}
	//for (s32 i = 0; i < MAX_UP_DOWN; ++i) {
	//	updown[i].texture = AEGfxTextureLoad("Assets/grassMid.png");
	//}

	//rectmesh = leftright1.mesh = updown1.mesh = diagup1.mesh = diagdown1.mesh = oneuse1.mesh = verti1.mesh = trapmesh = create_Square_Mesh();
	rectmesh = create_Square_Mesh();
	//rectmesh = leftright1.mesh = updown1.mesh = trapmesh = create_Square_Mesh();


}

void draw_level_init() {
	
	normal_blocks_create(6, -500, -200, 0);
	normal_blocks_create(5, 425, 0, 1);
	normal_blocks_create(5, 900, 350, 2);
	normal_blocks_create(5, 1400, 100, 3);
	normal_blocks_create(5, 2250, 500, 4);//
	normal_blocks_create(4, 3450, 850, 7);
	
	
	//normal_blocks_create(12, 2600, 0, 5);

	leftright_create(4, -150.f, -100.f, -150.f, 200.f, 0);

	updown_create(3, 700.f, 125.f, 125.f, 325.f, 0);
	updown_create(3, 2550.f, 600.f, 600.f, 775.f, 1);

	diagonal_up_create(4, 1700.f, 150.f, 1700.f, 2000.f, 150.f, 450.f, 0);
	diagonal_up_create(4, 3800.f, 900.f, 3800.f, 4100.f, 900.f, 1200.f, 1);

	diagonal_down_create(4, 900.f, 300.f, 900.f, 1150.f, 300.f, -50.f, 0);

	normal_blocks_create(4, 2750, 850, 5);
	normal_blocks_create(4, 3100, 850, 6);
	normal_blocks_create(4, 4400, 1300, 8);

	one_time_use_create(4, 100, 0, 0);
	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		onetimeuse[i].flag = ACTIVE;
		//onetimeuse[i].timer = 5;
	}




	//diagup1.x = 400.f;
	//diagup1.y = 100.f;
	//diagup1.end_x = 600.f;
	//diagup1.start_x = 100.f;
	//diagup1.end_y = 250.f;
	//diagup1.start_y = 100.f;

	//diagdown1.x = -250.f;
	//diagdown1.y = 250.f;
	//diagdown1.end_x = -150.f;
	//diagdown1.start_x = -250.f;
	//diagdown1.end_y = 150.f;
	//diagdown1.min_y = 250.f;
	
	//oneuse1.x = -400.f;
	//oneuse1.y = 150.f;
	//oneuse1.flag = ACTIVE;

	//droptrap1.x = -400.f;
	//droptrap1.y = 100.f;
	//droptrap1.start_y = 100.f;
	//droptrap1.end_y = -150.f;

	//verti1.x = -300.f;
	//verti1.y = -150.f;
	//verti1.end_x = -400.f;
	//verti1.start_x = -300.f;

	//horizontaltrap1.x = 100.f;
	//horizontaltrap1.y = 150.f;
	//horizontaltrap1.end_x = 0.f;
	//horizontaltrap1.start_x = 100.f;



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

	blocks_draw();
	leftright_blocks_draw();
	updown_blocks_draw();
	diag_up_blocks_draw();
	diag_down_blocks_draw();
	one_time_use_blocks_draw();

}

void normal_blocks_create(s32 len, f32 x, f32 y, s32 index) {
	normal[index].length = len;
	normal[index].x = x;
	normal[index].y = y;
}


void leftright_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index) {
	leftright[index].length = len;
	leftright[index].x = x;
	leftright[index].y = y;
	leftright[index].start_x = start_x;
	leftright[index].end_x = end_x;
	
}


void updown_create(s32 len, f32 x, f32 y, f32 start_y, f32 end_y, s32 index) {
	updown[index].length = len;
	updown[index].x = x;
	updown[index].y = y;
	updown[index].start_y = start_y;
	updown[index].end_y = end_y;
}


void diagonal_up_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index) {
	diagonalup[index].length = len;
	diagonalup[index].x = x;
	diagonalup[index].y = y;
	diagonalup[index].start_x = start_x;
	diagonalup[index].end_x = end_x;
	diagonalup[index].start_y = start_y;
	diagonalup[index].end_y = end_y;
}






void diagonal_down_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index) {
	diagonaldown[index].length = len;
	diagonaldown[index].x = x;
	diagonaldown[index].y = y;
	diagonaldown[index].start_x = start_x;
	diagonaldown[index].end_x = end_x;
	diagonaldown[index].start_y = start_y;
	diagonaldown[index].end_y = end_y;
}

void one_time_use_create(s32 len, f32 x, f32 y, s32 index) {
	onetimeuse[index].length = len;
	onetimeuse[index].x = x;
	onetimeuse[index].y = y;
}

void update_level() {
	// Creates an anti-gravity zone

	//anti_gravity_zone(5000, 5200);
	

	player.y -= GRAVITY;
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		enemy1[i].y -= GRAVITY;
	}
	move_update();



}

//-------------------- Parameters --------------------
// length -> how many blocks to draw
// x -> x coordinates of starting block (-500 to 500)
// y -> y coordinates of starting block (-300 to 300)
// ---------------------------------------------------
void blocks_draw() {
	for (s32 i = 0; i < MAX_NORMAL; i++) {
		for (s32 j = 0; j < normal[i].length; j++) {
			AEMtx33Scale(&scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + normal[i].x
				, BLOCK_HEIGHT / 2 + normal[i].y);
			// Concat the matrices
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);

			// Set the texture
			AEGfxTextureSet(rect, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms
			platform_collision(normal[i].length, normal[i].x, normal[i].y);
		}
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
		AEGfxTextureSet(upwardspike, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms
		AEVec2Set(&trapping.center, BLOCK_WIDTH / 2 + BLOCK_WIDTH * length + x , trapping.height);
		trap_collision(length, x, y);

	}
}

void dropping_spikes(s32 length, f32 x, f32 y) {

	droptrap1.length = length;
	droptrap1.x = x;
	droptrap1.y = y;


	for (s32 i = 0; i < length; i++) {
		droptrap1.width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * i + x;
		droptrap1.height = BLOCK_HEIGHT / 2 + y;

		AEMtx33Scale(&droptrap1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&droptrap1.rotate, PI);
		AEMtx33Trans(&droptrap1.translate, droptrap1.width, droptrap1.height);
		// Concat the matrices
		AEMtx33Concat(&droptrap1.transform, &droptrap1.rotate, &droptrap1.scale);
		AEMtx33Concat(&droptrap1.transform, &droptrap1.translate, &droptrap1.transform);
		AEGfxSetTransform(droptrap1.transform.m);

		// Set the texture
		AEGfxTextureSet(downwardspike, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		//trap_collision(length, x, y);


	}
	AEVec2Set(&droptrap1.center, (BLOCK_WIDTH * length) / 2 + x, y - BLOCK_HEIGHT * 1.2f);
}

void leftright_blocks_draw() {

	for (s32 i = 0; i < MAX_LEFT_RIGHT; i++) {
		for (s32 j = 0; j < leftright[i].length; j++) {
			leftright[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + leftright[i].x;
			leftright[i].height = BLOCK_HEIGHT / 2 + leftright[i].y;

			AEMtx33Scale(&leftright[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&leftright[i].rotate, PI);
			AEMtx33Trans(&leftright[i].translate, leftright[i].width, leftright[i].height);
			// Concat the matrices
			AEMtx33Concat(&leftright[i].transform, &leftright[i].rotate, &leftright[i].scale);
			AEMtx33Concat(&leftright[i].transform, &leftright[i].translate, &leftright[i].transform);
			AEGfxSetTransform(leftright[i].transform.m);

			// Set the texture
			AEGfxTextureSet(rect, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(leftright[i].length, leftright[i].x, leftright[i].y);
		}
		AEVec2Set(&leftright[i].center, (BLOCK_WIDTH * leftright[i].length) / 2 + leftright[i].x, leftright[i].y + BLOCK_HEIGHT / 4 * 3);

	}
	
}

void updown_blocks_draw() {

	for (s32 i = 0; i < MAX_UP_DOWN; i++) {
		for (s32 j = 0; j < updown[i].length; j++) {
			updown[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + updown[i].x;
			updown[i].height = BLOCK_HEIGHT / 2 + updown[i].y;

			AEMtx33Scale(&updown[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&updown[i].rotate, PI);
			AEMtx33Trans(&updown[i].translate, updown[i].width, updown[i].height);
			// Concat the matrices
			AEMtx33Concat(&updown[i].transform, &updown[i].rotate, &updown[i].scale);
			AEMtx33Concat(&updown[i].transform, &updown[i].translate, &updown[i].transform);
			AEGfxSetTransform(updown[i].transform.m);

			// Set the texture
			AEGfxTextureSet(rect, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(updown[i].length, updown[i].x, updown[i].y);


		}
		AEVec2Set(&updown[i].center, (BLOCK_WIDTH * updown[i].length) / 2 + updown[i].x, updown[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

void diag_up_blocks_draw() {


	for (s32 i = 0; i < MAX_DIAGONAL_UP; i++) {
		for (s32 j = 0; j < diagonalup[i].length; j++) {
			diagonalup[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + diagonalup[i].x;
			diagonalup[i].height = BLOCK_HEIGHT / 2 + diagonalup[i].y;

			AEMtx33Scale(&diagonalup[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&diagonalup[i].rotate, PI);
			AEMtx33Trans(&diagonalup[i].translate, diagonalup[i].width, diagonalup[i].height);
			// Concat the matrices
			AEMtx33Concat(&diagonalup[i].transform, &diagonalup[i].rotate, &diagonalup[i].scale);
			AEMtx33Concat(&diagonalup[i].transform, &diagonalup[i].translate, &diagonalup[i].transform);
			AEGfxSetTransform(diagonalup[i].transform.m);

			// Set the texture
			AEGfxTextureSet(rect, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(diagonalup[i].length, diagonalup[i].x, diagonalup[i].y);


		}
		AEVec2Set(&diagonalup[i].center, (BLOCK_WIDTH * diagonalup[i].length) / 2 + diagonalup[i].x, diagonalup[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

void diag_down_blocks_draw() {
	for (s32 i = 0; i < MAX_DIAGONAL_DOWN; i++) {
		for (s32 j = 0; j < diagonaldown[i].length; j++) {
			diagonaldown[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + diagonaldown[i].x;
			diagonaldown[i].height = BLOCK_HEIGHT / 2 + diagonaldown[i].y;

			AEMtx33Scale(&diagonaldown[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&diagonaldown[i].rotate, PI);
			AEMtx33Trans(&diagonaldown[i].translate, diagonaldown[i].width, diagonaldown[i].height);
			// Concat the matrices
			AEMtx33Concat(&diagonaldown[i].transform, &diagonaldown[i].rotate, &diagonaldown[i].scale);
			AEMtx33Concat(&diagonaldown[i].transform, &diagonaldown[i].translate, &diagonaldown[i].transform);
			AEGfxSetTransform(diagonaldown[i].transform.m);

			// Set the texture
			AEGfxTextureSet(rect, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(diagonaldown[i].length, diagonaldown[i].x, diagonaldown[i].y);


		}
		AEVec2Set(&diagonaldown[i].center, (BLOCK_WIDTH * diagonaldown[i].length) / 2 + diagonaldown[i].x, diagonaldown[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

void one_time_use_blocks_draw() {

	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		for (s32 j = 0; j < onetimeuse[i].length; j++) {
			onetimeuse[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + onetimeuse[i].x;
			onetimeuse[i].height = BLOCK_HEIGHT / 2 + onetimeuse[i].y;

			AEMtx33Scale(&onetimeuse[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&onetimeuse[i].rotate, PI);
			AEMtx33Trans(&onetimeuse[i].translate, onetimeuse[i].width, onetimeuse[i].height);
			// Concat the matrices
			AEMtx33Concat(&onetimeuse[i].transform, &onetimeuse[i].rotate, &onetimeuse[i].scale);
			AEMtx33Concat(&onetimeuse[i].transform, &onetimeuse[i].translate, &onetimeuse[i].transform);
			AEGfxSetTransform(onetimeuse[i].transform.m);

			// Set the texture
			AEGfxTextureSet(rect, 0, 0);
			if (onetimeuse[i].flag == ACTIVE) {
				AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
				// Player collision with platforms
				platform_collision(onetimeuse[i].length, onetimeuse[i].x, onetimeuse[i].y);
			}

		}
		AEVec2Set(&onetimeuse[i].center, (BLOCK_WIDTH * onetimeuse[i].length) / 2 + onetimeuse[i].x, onetimeuse[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}


void verti_blocks(s32 length, f32 x, f32 y) {

	verti1.length = length;
	verti1.x = x;
	verti1.y = y;


	for (s32 i = 0; i < length; i++) {
		verti1.width = BLOCK_WIDTH / 2 + x;
		verti1.height = BLOCK_HEIGHT / 2 + BLOCK_WIDTH * i + y;

		AEMtx33Scale(&verti1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&verti1.rotate, PI);
		AEMtx33Trans(&verti1.translate, verti1.width, verti1.height);
		// Concat the matrices
		AEMtx33Concat(&verti1.transform, &verti1.rotate, &verti1.scale);
		AEMtx33Concat(&verti1.transform, &verti1.translate, &verti1.transform);
		AEGfxSetTransform(verti1.transform.m);

		// Set the texture
		AEGfxTextureSet(rect, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		verti_collision(length, x, y);


	}
	AEVec2Set(&verti1.center, BLOCK_WIDTH + x, (BLOCK_HEIGHT * length) / 2 + y);
}

void left_right_blades(s32 length, f32 x, f32 y) {

	horizontaltrap1.length = length;
	horizontaltrap1.x = x;
	horizontaltrap1.y = y;


	for (s32 i = 0; i < length; i++) {
		horizontaltrap1.width = BLOCK_WIDTH / 2 + x;
		horizontaltrap1.height = BLOCK_HEIGHT / 2 + BLOCK_WIDTH * i + y;

		AEMtx33Scale(&horizontaltrap1.scale, BLOCK_WIDTH, BLOCK_HEIGHT);
		AEMtx33Rot(&horizontaltrap1.rotate, PI);
		AEMtx33Trans(&horizontaltrap1.translate, horizontaltrap1.width, horizontaltrap1.height);
		// Concat the matrices
		AEMtx33Concat(&horizontaltrap1.transform, &horizontaltrap1.rotate, &horizontaltrap1.scale);
		AEMtx33Concat(&horizontaltrap1.transform, &horizontaltrap1.translate, &horizontaltrap1.transform);
		AEGfxSetTransform(horizontaltrap1.transform.m);

		// Set the texture
		AEGfxTextureSet(downwardspike, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		// Player collision with platforms

		//verti_collision(length, x, y);


	}
	AEVec2Set(&horizontaltrap1.center, BLOCK_WIDTH + x, (BLOCK_HEIGHT * length) / 2 + y);
}

void move_update() {
	//in case of using sine
	f32* time = nullptr;
	static bool isIncrementing = false;
	//move1.x = 200 * sinf(static_cast<float>(AEGetTime(time)/0.5f));
	//if the x value is now == original position 
	for (s32 i = 0; i < MAX_LEFT_RIGHT; i++) {
		if (leftright[i].pos == OG) {
			leftright[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&leftright[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (leftright[i].x >= leftright[i].end_x) {
				leftright[i].pos = MOVED;
			}
		}

		if (leftright[i].pos == MOVED) {
			leftright[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&leftright[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (leftright[i].x <= leftright[i].start_x) {
				leftright[i].pos = OG;
			}
		}
	}

	for (s32 i = 0; i < MAX_UP_DOWN; i++) {
		if (updown[i].pos == OG) {
			updown[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&updown[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				player.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				//std::cout << "collided for up down" << std::endl;
			}
			if (updown[i].y >= updown[i].end_y) {
				updown[i].pos = MOVED;
			}
		}

		if (updown[i].pos == MOVED) {
			updown[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&updown[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (updown[i].y <= updown[i].start_y) {
				updown[i].pos = OG;
			}
		}
	}

	for (s32 i = 0; i < MAX_DIAGONAL_UP; i++) {
		if (diagonalup[i].pos == OG) {
			diagonalup[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			diagonalup[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&diagonalup[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				player.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				player.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				std::cout << "collided for diag up" << std::endl;
			}
			if (diagonalup[i].y >= diagonalup[i].end_y) { //&& diagup1.x >= diagup1.end_x
				diagonalup[i].pos = MOVED;
			}
		}

		if (diagonalup[i].pos == MOVED) {
			diagonalup[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			diagonalup[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&diagonalup[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				player.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				player.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			}
			if (diagonalup[i].y <= diagonalup[i].start_y) {
				diagonalup[i].pos = OG;
			}
		}
	}

	for (s32 i = 0; i < MAX_DIAGONAL_DOWN; i++) {
		if (diagonaldown[i].pos == OG) {
			diagonaldown[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			diagonaldown[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&diagonaldown[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				player.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				player.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				//std::cout << "collided with diag down" << std::endl;
			}
			if (diagonaldown[i].y <= diagonaldown[i].end_y) {//&& diagdown1.x >= diagdown1.end_x
				diagonaldown[i].pos = MOVED;
			}
		}

		if (diagonaldown[i].pos == MOVED) {
			diagonaldown[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			diagonaldown[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&diagonaldown[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				player.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				player.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			}
			if (diagonaldown[i].y >= diagonaldown[i].start_y) {
				diagonaldown[i].pos = OG;
			}
		}
	}

	//for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
	//	if (AETestRectToRect(&onetimeuse[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
	//		isIncrementing = TRUE;
	//		if(isIncrementing == TRUE) onetimeuse[i].timer += AEFrameRateControllerGetFrameTime();
	//		if (onetimeuse[i].timer >= 5) {
	//			isIncrementing = FALSE;
	//			onetimeuse[i].flag = NOT_ACTIVE;
	//		}
	//	}
	//}
	//for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
	//	if (onetimeuse[i].pos == OG) {
	//		if (AETestRectToRect(&onetimeuse[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
	//			onetimeuse[i].timer -= 1;
	//			if (onetimeuse[i].timer <= 0) onetimeuse[i].flag = NOT_ACTIVE;
	//		}
	//	}
	//}
	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		if (AETestRectToRect(&onetimeuse[i].center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			onetimeuse[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime());
			if (onetimeuse[i].timer >= 5) onetimeuse[i].flag = NOT_ACTIVE;
		}
	}

	if (verti1.flag == ACTIVE) {
		if (verti1.pos == OG) {
			verti1.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&verti1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				player.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
				//std::cout << "colided" << std::endl;
			}
			if (verti1.x <= verti1.end_x) {
				verti1.pos = MOVED;
			}
		}

		if (verti1.pos == MOVED) {
			verti1.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			//if (AETestRectToRect(&verti1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
			if (verti1.x >= verti1.start_x) {
				verti1.pos = OG;
			}
		}
	}

	if (droptrap1.flag == ACTIVE) {
		if (droptrap1.pos == OG) {
			droptrap1.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * dropSpeed;
			if (AETestRectToRect(&droptrap1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				if (damage_ok == TRUE) {
					--player.Hp;
					damage_ok = FALSE;
					//call transparancy function(?) to show invincibility
				}
				else if (damage_ok == FALSE) {
					if (AEFrameRateControllerGetFrameCount() % 50 == 0) {
						damage_ok = TRUE;
						//set transparacny function(?) to false
					}

				}

			}//player.y += AEFrameRateControllerGetFrameTime() * moveSpeed;
			if (droptrap1.y <= droptrap1.end_y) {
				droptrap1.pos = MOVED;
			}
		}


		if (droptrap1.pos == MOVED) {
			droptrap1.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			//if (AETestRectToRect(&droptrap1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.y -= AEFrameRateControllerGetFrameTime() * moveSpeed;
			if (droptrap1.y >= droptrap1.start_y) {
				droptrap1.pos = OG;
			}
		}
	}

	if (horizontaltrap1.flag == ACTIVE) {
		if (horizontaltrap1.pos == OG) {
			horizontaltrap1.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			if (AETestRectToRect(&horizontaltrap1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
				if (damage_ok == TRUE) {
					--player.Hp;
					//std::cout << "colided" << std::endl;
					damage_ok = FALSE;
					//call transparancy function(?) to show invincibility
				}
				else if (damage_ok == FALSE) {
					if (AEFrameRateControllerGetFrameCount() % 50 == 0) {
						damage_ok = TRUE;
						//set transparacny function(?) to false
					}

				}
			}
			if (horizontaltrap1.x <= horizontaltrap1.end_x) {
				horizontaltrap1.pos = MOVED;
			}
		}

		if (horizontaltrap1.pos == MOVED) {
			horizontaltrap1.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
			//if (AETestRectToRect(&verti1.center, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) player.x += AEFrameRateControllerGetFrameTime() * moveSpeed;
			if (horizontaltrap1.x >= horizontaltrap1.start_x) {
				horizontaltrap1.pos = OG;
			}
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

void verti_collision(s32 cnt, f32 x, f32 y) {
	for (f32 i = 0; i < cnt; i++) {
		//if ((player.x <= BLOCK_WIDTH + x + PLAYER_WIDTH / 2) &&
		//	(player.x >= x + PLAYER_WIDTH / 2) &&
		//	(player.y <= BLOCK_HEIGHT / 2 + BLOCK_HEIGHT * cnt + y - PLAYER_HEIGHT / 2) &&
		//	(player.y >= y + BLOCK_HEIGHT / 2 - PLAYER_HEIGHT / 2))
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
void anti_gravity_zone(f32 x1, f32 x2) {
	
	if (player.x >= x1 && player.x <= x2)
		player.y += GRAVITY;
	else // player <= x2
		player.y -= GRAVITY;

}

void draw_anti_gravity_zone(f32 x1, f32 x2) {

	/*AEGfxSetTransparency(0.55f);
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/
}










