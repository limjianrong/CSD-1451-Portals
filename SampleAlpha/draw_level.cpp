/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					draw_level.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Tay Zhun Hang (zhunhang.tay@digipen.edu)
*
* Brief:
  This source file implements the functions used for the level design of the game. It
  also includes the source code for the logic of different platforms types.
==================================================================================*/
#include "draw_level.hpp"

AEGfxTexture* platform_text, * spike_text, *onetime_text;
AEMtx33 scale, rotate, translate, transform; // TRS
extern AEGfxVertexList* square_mesh;	// Created square mesh

Block normal[MAX_NORMAL], leftright[MAX_LEFT_RIGHT], updown[MAX_UP_DOWN], diagonalup[MAX_DIAGONAL_UP],
diagonaldown[MAX_DIAGONAL_DOWN], onetimeuse[MAX_ONE_TIME_USE],floorspikes[MAX_SPIKES], leftrightspikes[MAX_LEFT_RIGHT_SPIKES];
Door door;

extern Player_stats player;
extern Enemy1_stats enemy1[MAX_ENEMIES_1];
extern AEAudio playerDamageAudio;
extern AEAudioGroup soundGroup;

bool damage_ok{ TRUE };

// file IO
std::ifstream door_ifs{};

// NOTE: GRAVITY, BLOCK_WIDTH, BLOCK_HEIGHT defined in .hpp

// load all textures
void draw_level_load() {
	platform_text = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile001.png");
	onetime_text = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile013.png");
	spike_text = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Other/spikesHigh.png");
	door.picture = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile052.png");
	door_ifs.open("Assets/textFiles/door.txt");
	std::string str{};
	door_ifs >> str >> door.center.x;
	door_ifs >> str >> door.center.y;
	door_ifs >> str >> door.width;
	door_ifs >> str >> door.height;
}

// initialise values for the platforms
void draw_level_init() {

	// spikes
	spikes_create(2, 1550, 150, 0);
	left_right_spikes_create(1, 4800, 525, 4800, 5450, 0);
	left_right_spikes_create(1, 4950, 525, 4950, 5600, 1);

	// normal platforms for the top route
	normal_blocks_create(6, -500, -200, 0);
	normal_blocks_create(5, 425, 0, 1);
	normal_blocks_create(5, 900, 350, 2);
	normal_blocks_create(5, 1400, 125, 3);
	normal_blocks_create(5, 2250, 500, 4);
	normal_blocks_create(6, 3450, 850, 5);
	normal_blocks_create(7, 5400, 950, 6);
	normal_blocks_create(7, 6550, 1000, 7);
	normal_blocks_create(4, 7000, 750, 8);

	// normal platforms for the bottom route
	normal_blocks_create(4, 800, -150, 9);
	normal_blocks_create(4, 2550, -250, 10);
	normal_blocks_create(6, 3350, 200, 11);
	normal_blocks_create(5, 4200, 300, 12);
	normal_blocks_create(4, 5800, 100, 13);
	normal_blocks_create(7, 6900, 300, 14);

	// normal platforms for boss fight
	normal_blocks_create(20, 7600, 450, 15); 
	normal_blocks_create(7, 7300, 200, 16);
	normal_blocks_create(7, 7300, 625, 17);

	// platform that move left and right for top route
	leftright_create(4, -150, -100, -150, 200, 0);
	leftright_create(5, 5850, 950, 5850, 6250, 1);

	// platform that move left and right for bottom route	
	leftright_create(4, 1050, -200, 1050, 1850, 2);
	leftright_create(4, 4800, 500, 4800, 5450, 3);
	leftright_create(4, 6150, 200, 6150, 6650, 4);
	
	// platform that move up and down for top route
	updown_create(3, 700, 125, 125, 325, 0);
	updown_create(3, 2550, 600, 600, 775, 1);

	// platform that move up and down for bottom route
	updown_create(3, 4550, 400, 400, 650, 2);

	// platform that move diagonally upwards for top route
	diagonal_up_create(4, 1700, 150, 1700, 2000, 150, 450, 0);
	diagonal_up_create(4, 3800, 900, 3800, 4100, 900, 1200, 1);
	
	// platform that move diagonally upwards for bottom route
	diagonal_up_create(4, 2000, -150, 2000, 2300, -150, 150, 2);
	diagonal_up_create(3, 2800, -200, 2800, 3150, -200, 150, 3);

	// platform that move diagonally downwards
	diagonal_down_create(4, 4700, 1200, 4700, 5050, 1200, 850, 0);

	// platforms that will disppear for top route
	one_time_use_create(4, 2750, 850, 0);
	one_time_use_create(4, 3100, 850, 1);
	one_time_use_create(4, 4400, 1300, 2);

	// platforms that will disppear for bottom route
	one_time_use_create(4, 2550, 175, 3);
	one_time_use_create(4, 3800, 300, 4);
	one_time_use_create(4, 5800, 500, 5);

	// initialise the one time use flags
	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		onetimeuse[i].flag = ACTIVE;
	}
}

// draw function for draw various platforms and objects
void draw_level_draw() {
	spikes_draw();
	left_right_spikes_draw();
	blocks_draw();
	leftright_blocks_draw();
	updown_blocks_draw();
	diag_up_blocks_draw();
	diag_down_blocks_draw();
	one_time_use_blocks_draw();
	door_draw();
}

// takes in values initialised and store it in struct array for the normal platforms
void normal_blocks_create(s32 len, f32 x, f32 y, s32 index) {
	normal[index].length = len;
	normal[index].x = x;
	normal[index].y = y;
}

// takes in values initialised and store it in struct array for the platforms that move left and right
void leftright_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index) {
	leftright[index].length = len;
	leftright[index].x = x;
	leftright[index].y = y;
	leftright[index].start_x = start_x;
	leftright[index].end_x = end_x;
}

// takes in values initialised and store it in struct array for the platforms that move up and down
void updown_create(s32 len, f32 x, f32 y, f32 start_y, f32 end_y, s32 index) {
	updown[index].length = len;
	updown[index].x = x;
	updown[index].y = y;
	updown[index].start_y = start_y;
	updown[index].end_y = end_y;
}

// takes in values initialised and store it in struct array for the platforms that move diagonally upwards
void diagonal_up_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index) {
	diagonalup[index].length = len;
	diagonalup[index].x = x;
	diagonalup[index].y = y;
	diagonalup[index].start_x = start_x;
	diagonalup[index].end_x = end_x;
	diagonalup[index].start_y = start_y;
	diagonalup[index].end_y = end_y;
}

// takes in values initialised and store it in struct array for the platforms that move diagonally downwards
void diagonal_down_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index) {
	diagonaldown[index].length = len;
	diagonaldown[index].x = x;
	diagonaldown[index].y = y;
	diagonaldown[index].start_x = start_x;
	diagonaldown[index].end_x = end_x;
	diagonaldown[index].start_y = start_y;
	diagonaldown[index].end_y = end_y;
}

// takes in values initialised and store it in struct array for the platforms that will disappear
void one_time_use_create(s32 len, f32 x, f32 y, s32 index) {
	onetimeuse[index].length = len;
	onetimeuse[index].x = x;
	onetimeuse[index].y = y;
}

// takes in values initialised and store it in struct array for the spikes that damages player
void spikes_create(s32 len, f32 x, f32 y, s32 index) {
	floorspikes[index].length = len;
	floorspikes[index].x = x;
	floorspikes[index].y = y;
}

// takes in values initialised and store it in struct array for the spikes that will move together
// with the platform and damages player
void left_right_spikes_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index) {
	leftrightspikes[index].length = len;
	leftrightspikes[index].x = x;
	leftrightspikes[index].y = y;
	leftrightspikes[index].start_x = start_x;
	leftrightspikes[index].end_x = end_x;
}
// update function. contains the source codes that implements gravity and
// logic for different platforms/spikes
void draw_level_update() {
	player.center.y -= GRAVITY;

	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		enemy1[i].center.y -= GRAVITY;
	}
	platform_logic();
}

//free function
void draw_level_free() {

}

//unload function. unloads all textures and meshes
void draw_level_unload() {
	// Texture unload
	AEGfxTextureUnload(platform_text);
	AEGfxTextureUnload(spike_text);
	AEGfxTextureUnload(onetime_text);
	AEGfxTextureUnload(door.picture);
}

// Normal Platform Draw Function
void blocks_draw() {
	for (s32 i = 0; i < MAX_NORMAL; i++) {
		for (s32 j = 0; j < normal[i].length; j++) {
			AEMtx33Scale(&scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&rotate, NULL);
			AEMtx33Trans(&translate, BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + normal[i].x
				, BLOCK_HEIGHT / 2 + normal[i].y);
			// Concat the matrices
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms
			platform_collision(normal[i].length, normal[i].x, normal[i].y);
		}
	}
}

// Spikes Draw Function
void spikes_draw() {
	for (s32 i = 0; i < MAX_SPIKES; i++) {
		for (s32 j = 0; j < floorspikes[i].length; j++) {
			floorspikes[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + floorspikes[i].x;
			floorspikes[i].height = BLOCK_HEIGHT / 2 + floorspikes[i].y;

			AEMtx33Scale(&floorspikes[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&floorspikes[i].rotate, NULL);
			AEMtx33Trans(&floorspikes[i].translate, floorspikes[i].width, floorspikes[i].height);
			// Concat the matrices
			AEMtx33Concat(&floorspikes[i].transform, &floorspikes[i].rotate, &floorspikes[i].scale);
			AEMtx33Concat(&floorspikes[i].transform, &floorspikes[i].translate, &floorspikes[i].transform);
			AEGfxSetTransform(floorspikes[i].transform.m);
			// Set the texture
			AEGfxTextureSet(spike_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		}
		// Set the center
		AEVec2Set(&floorspikes[i].center, (BLOCK_WIDTH * floorspikes[i].length) / 2 + floorspikes[i].x, floorspikes[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// Moving Left & Right Platform Draw Function
void leftright_blocks_draw() {

	for (s32 i = 0; i < MAX_LEFT_RIGHT; i++) {
		for (s32 j = 0; j < leftright[i].length; j++) {
			leftright[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + leftright[i].x;
			leftright[i].height = BLOCK_HEIGHT / 2 + leftright[i].y;

			AEMtx33Scale(&leftright[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&leftright[i].rotate, NULL);
			AEMtx33Trans(&leftright[i].translate, leftright[i].width, leftright[i].height);
			// Concat the matrices
			AEMtx33Concat(&leftright[i].transform, &leftright[i].rotate, &leftright[i].scale);
			AEMtx33Concat(&leftright[i].transform, &leftright[i].translate, &leftright[i].transform);
			AEGfxSetTransform(leftright[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(leftright[i].length, leftright[i].x, leftright[i].y);
		}
		// Set the center
		AEVec2Set(&leftright[i].center, (BLOCK_WIDTH * leftright[i].length) / 2 + leftright[i].x, leftright[i].y + BLOCK_HEIGHT / 4 * 3);

	}
	
}

// Moving Up & Down Platform Draw Function
void updown_blocks_draw() {

	for (s32 i = 0; i < MAX_UP_DOWN; i++) {
		for (s32 j = 0; j < updown[i].length; j++) {
			updown[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + updown[i].x;
			updown[i].height = BLOCK_HEIGHT / 2 + updown[i].y;

			AEMtx33Scale(&updown[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&updown[i].rotate, NULL);
			AEMtx33Trans(&updown[i].translate, updown[i].width, updown[i].height);
			// Concat the matrices
			AEMtx33Concat(&updown[i].transform, &updown[i].rotate, &updown[i].scale);
			AEMtx33Concat(&updown[i].transform, &updown[i].translate, &updown[i].transform);
			AEGfxSetTransform(updown[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms
			platform_collision(updown[i].length, updown[i].x, updown[i].y);


		}
		AEVec2Set(&updown[i].center, (BLOCK_WIDTH * updown[i].length) / 2 + updown[i].x, updown[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// Moving Diagonally Upward Platform Draw Function
void diag_up_blocks_draw() {

	for (s32 i = 0; i < MAX_DIAGONAL_UP; i++) {
		for (s32 j = 0; j < diagonalup[i].length; j++) {
			diagonalup[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + diagonalup[i].x;
			diagonalup[i].height = BLOCK_HEIGHT / 2 + diagonalup[i].y;

			AEMtx33Scale(&diagonalup[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&diagonalup[i].rotate, NULL);
			AEMtx33Trans(&diagonalup[i].translate, diagonalup[i].width, diagonalup[i].height);
			// Concat the matrices
			AEMtx33Concat(&diagonalup[i].transform, &diagonalup[i].rotate, &diagonalup[i].scale);
			AEMtx33Concat(&diagonalup[i].transform, &diagonalup[i].translate, &diagonalup[i].transform);
			AEGfxSetTransform(diagonalup[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms
			platform_collision(diagonalup[i].length, diagonalup[i].x, diagonalup[i].y);
		}
		// Set the center
		AEVec2Set(&diagonalup[i].center, (BLOCK_WIDTH * diagonalup[i].length) / 2 + diagonalup[i].x, diagonalup[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// Moving Diagonally Downward Platform Draw Function
void diag_down_blocks_draw() {
	for (s32 i = 0; i < MAX_DIAGONAL_DOWN; i++) {
		for (s32 j = 0; j < diagonaldown[i].length; j++) {
			diagonaldown[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + diagonaldown[i].x;
			diagonaldown[i].height = BLOCK_HEIGHT / 2 + diagonaldown[i].y;

			AEMtx33Scale(&diagonaldown[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&diagonaldown[i].rotate, NULL);
			AEMtx33Trans(&diagonaldown[i].translate, diagonaldown[i].width, diagonaldown[i].height);
			// Concat the matrices
			AEMtx33Concat(&diagonaldown[i].transform, &diagonaldown[i].rotate, &diagonaldown[i].scale);
			AEMtx33Concat(&diagonaldown[i].transform, &diagonaldown[i].translate, &diagonaldown[i].transform);
			AEGfxSetTransform(diagonaldown[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(diagonaldown[i].length, diagonaldown[i].x, diagonaldown[i].y);
		}
		// Set the center
		AEVec2Set(&diagonaldown[i].center, (BLOCK_WIDTH * diagonaldown[i].length) / 2 + diagonaldown[i].x, diagonaldown[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// Platform That Will Disappear Draw Function
void one_time_use_blocks_draw() {

	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		for (s32 j = 0; j < onetimeuse[i].length; j++) {
			onetimeuse[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + onetimeuse[i].x;
			onetimeuse[i].height = BLOCK_HEIGHT / 2 + onetimeuse[i].y;

			AEMtx33Scale(&onetimeuse[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&onetimeuse[i].rotate, NULL);
			AEMtx33Trans(&onetimeuse[i].translate, onetimeuse[i].width, onetimeuse[i].height);
			// Concat the matrices
			AEMtx33Concat(&onetimeuse[i].transform, &onetimeuse[i].rotate, &onetimeuse[i].scale);
			AEMtx33Concat(&onetimeuse[i].transform, &onetimeuse[i].translate, &onetimeuse[i].transform);
			AEGfxSetTransform(onetimeuse[i].transform.m);

			// Set the texture
			AEGfxTextureSet(onetime_text, 0, 0);
			if (onetimeuse[i].flag == ACTIVE) {
				AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
				// Player collision with platforms
				platform_collision(onetimeuse[i].length, onetimeuse[i].x, onetimeuse[i].y);
			}
		}
		// Set the center
		AEVec2Set(&onetimeuse[i].center, (BLOCK_WIDTH * onetimeuse[i].length) / 2 + onetimeuse[i].x, onetimeuse[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// Spikes that will move together with the moving left & right platforms Draw Function
void left_right_spikes_draw() {

	for (s32 i = 0; i < MAX_LEFT_RIGHT_SPIKES; i++) {
		for (s32 j = 0; j < leftrightspikes[i].length; j++) {
			leftrightspikes[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + leftrightspikes[i].x;
			leftrightspikes[i].height = BLOCK_HEIGHT / 2 + leftrightspikes[i].y;

			AEMtx33Scale(&leftrightspikes[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&leftrightspikes[i].rotate, NULL);
			AEMtx33Trans(&leftrightspikes[i].translate, leftrightspikes[i].width, leftrightspikes[i].height);
			// Concat the matrices
			AEMtx33Concat(&leftrightspikes[i].transform, &leftrightspikes[i].rotate, &leftrightspikes[i].scale);
			AEMtx33Concat(&leftrightspikes[i].transform, &leftrightspikes[i].translate, &leftrightspikes[i].transform);
			AEGfxSetTransform(leftrightspikes[i].transform.m);

			// Set the texture
			AEGfxTextureSet(spike_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
		AEVec2Set(&leftrightspikes[i].center, (BLOCK_WIDTH * leftrightspikes[i].length) / 2 + leftrightspikes[i].x, leftrightspikes[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// Door Draw Function
void door_draw() {
	AEMtx33Scale(&door.scale, door.width, door.height);
	AEMtx33Trans(&door.translate, door.center.x, door.center.y);
	AEMtx33Concat(&door.final, &door.translate, &door.scale);
	AEGfxSetTransform(door.final.m);
	AEGfxTextureSet(door.picture, 0.0f, 0.0f);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}

// this function contains the codes for the different logics of the different platforms types
void platform_logic() {
	
	// logic for moving left & right platform
	for (s32 i = 0; i < MAX_LEFT_RIGHT; i++) {
		//checks if the platform is at its original position
		if (leftright[i].pos == OG) {
			leftright[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves platform
			// checks if player is standing on the platform
			if (AETestRectToRect(&leftright[i].center, BLOCK_WIDTH * leftright[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				player.center.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves player together with platform
			}
			// checks if current position of platform reached the desired end point 
			if (leftright[i].x >= leftright[i].end_x) {
				leftright[i].pos = MOVED; // set platform position to MOVED status
			}
		}
		//checks if the platform is at its moved positoin
		if (leftright[i].pos == MOVED) {
			leftright[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves platform
			// checks if player is standing on the platform
			if (AETestRectToRect(&leftright[i].center, BLOCK_WIDTH * leftright[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				player.center.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves player together with platform
			}
			// checks if current position of platform reached the original start point 
			if (leftright[i].x <= leftright[i].start_x) {
				leftright[i].pos = OG; // set platform position to Original status
			}
		}
	}

	// logic for moving up & down platform
	for (s32 i = 0; i < MAX_UP_DOWN; i++) {
		// checks if the platform is at its original position
		if (updown[i].pos == OG) {
			updown[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;  // moves platform
			// checks if player is standing on the platform
			if (AETestRectToRect(&updown[i].center, BLOCK_WIDTH * updown[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				player.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves player together with platform
			}
			// checks if current position of platform reached the desired end point 
			if (updown[i].y >= updown[i].end_y) {
				updown[i].pos = MOVED;
			}
		}
		// checks if the platform is at its moved positoin
		if (updown[i].pos == MOVED) {
			updown[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;  // moves platform
			// checks if player is standing on the platform
			if (AETestRectToRect(&updown[i].center, BLOCK_WIDTH * updown[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) 
				player.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves player together with platform
			// checks if current position of platform reached the original start point 
			if (updown[i].y <= updown[i].start_y) {
				updown[i].pos = OG;
			}
		}
	}

	// logic for moving diagonally upward platform
	for (s32 i = 0; i < MAX_DIAGONAL_UP; i++) {
		// checks if the platform is at its original position
		if (diagonalup[i].pos == OG) {
			// moves platform
			diagonalup[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonalup[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonalup[i].center, BLOCK_WIDTH * diagonalup[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				// moves player together with platform
				player.center.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}
			// checks if current position of platform reached the desired end point 
			if (diagonalup[i].y >= diagonalup[i].end_y) { 
				diagonalup[i].pos = MOVED;
			}
		}
		// checks if the platform is at its moved positoin
		if (diagonalup[i].pos == MOVED) {
			// moves platform
			diagonalup[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonalup[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonalup[i].center, BLOCK_WIDTH * diagonalup[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				// moves player together with platform
				player.center.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}
			// checks if current position of platform reached the original start point 
			if (diagonalup[i].y <= diagonalup[i].start_y) {
				diagonalup[i].pos = OG;
			}
		}
	}

	// logic for moving diagonally downward platform
	for (s32 i = 0; i < MAX_DIAGONAL_DOWN; i++) {
		// checks if the platform is at its original position
		if (diagonaldown[i].pos == OG) {
			// moves platform
			diagonaldown[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonaldown[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonaldown[i].center, BLOCK_WIDTH * diagonaldown[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				// moves player together with platform
				player.center.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				}
			// checks if current position of platform reached the desired end point 
			if (diagonaldown[i].y <= diagonaldown[i].end_y) {//&& diagdown1.x >= diagdown1.end_x
				diagonaldown[i].pos = MOVED;
			}
		}
		// checks if the platform is at its moved positoin
		if (diagonaldown[i].pos == MOVED) {
			// moves platform
			diagonaldown[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonaldown[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonaldown[i].center, BLOCK_WIDTH * diagonaldown[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				// moves player together with platform
				player.center.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}
			// checks if current position of platform reached the original start point 
			if (diagonaldown[i].y >= diagonaldown[i].start_y) {
				diagonaldown[i].pos = OG;
			}
		}
	}

	// logic for disappearing platform
	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		// checks if player is standing on the platform
		if (AETestRectToRect(&onetimeuse[i].center, BLOCK_WIDTH * onetimeuse[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
			onetimeuse[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime()); // starts the timer to despawn the platform
			// checks if 3 seconds or more after player stepped on platform
			if (onetimeuse[i].timer >= 3) {
				if (onetimeuse[i].flag == ACTIVE) {
					// set flag for platform to not active
					onetimeuse[i].flag = NOT_ACTIVE;
					onetimeuse[i].timer = 0; // resets timer
				}
			}
		}
		else if (onetimeuse[i].timer > 0) { // checks if timer has started
			onetimeuse[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime()); //continue the timer
			// checks if 3 seconds or more after player stepped on platform
			if (onetimeuse[i].timer >= 3) {
				if (onetimeuse[i].flag == ACTIVE) {
					// set flag for platform to not active
					onetimeuse[i].flag = NOT_ACTIVE;
					onetimeuse[i].timer = 0; // resets timer
				}
			}
		}
		// checks if platform flag is no longer active
		if (onetimeuse[i].flag == NOT_ACTIVE) {
			// starts the timer to respawn the platform
			onetimeuse[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime());
			if (onetimeuse[i].timer >= 3) { 
				onetimeuse[i].timer = 0; 
				onetimeuse[i].flag = ACTIVE; // respawns the platform
			}
		}
	}

	// logic for spikes
	for (s32 i = 0; i < MAX_SPIKES; i++) {
		// checks if player is standing in the spikes
		if (AETestRectToRect(&floorspikes[i].center, BLOCK_WIDTH * floorspikes[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
			// check if damage is allowed
			if (damage_ok == TRUE) {
				--player.Hp; // decrease player health
				AEAudioPlay(playerDamageAudio, soundGroup, 0.25f, 1.f, 0); // player take damage sfx
				damage_ok = FALSE;
			}
			else if (damage_ok == FALSE) {
				if (AEFrameRateControllerGetFrameCount() % 50 == 0)
					damage_ok = TRUE; // allow player take damage 
			}
		}
	}

	// logic for moving left & right spikes
	for (s32 i = 0; i < MAX_LEFT_RIGHT_SPIKES; i++) {
		// checks if the spikes is at its original position
		if (leftrightspikes[i].pos == OG) {
			// moves spikes
			leftrightspikes[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			if (AETestRectToRect(&leftrightspikes[i].center, BLOCK_WIDTH * leftrightspikes[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				// check if damage is allowed
				if (damage_ok == TRUE) {
					--player.Hp; // decrease player health
					AEAudioPlay(playerDamageAudio, soundGroup, 0.25f, 1.f, 0); // player take damage sfx
					damage_ok = FALSE;
				}
				else if (damage_ok == FALSE) {
					if (AEFrameRateControllerGetFrameCount() % 50 == 0)
						damage_ok = TRUE; // allow player take damage 
				}
			}
			// checks if current position of platform reached the desired end point 
			if (leftrightspikes[i].x >= leftrightspikes[i].end_x) {
				leftrightspikes[i].pos = MOVED;
			}
		}
		// checks if the spikes is at its moved positoin
		if (leftrightspikes[i].pos == MOVED) {
			// moves spikes
			leftrightspikes[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			if (AETestRectToRect(&leftrightspikes[i].center, BLOCK_WIDTH * leftrightspikes[i].length, BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				// check if damage is allowed
				if (damage_ok == TRUE) {
					--player.Hp; // decrease player health
					AEAudioPlay(playerDamageAudio, soundGroup, 0.25f, 1.f, 0); // player take damage sfx
					damage_ok = FALSE;
				}
				else if (damage_ok == FALSE) {
					if (AEFrameRateControllerGetFrameCount() % 50 == 0)
						damage_ok = TRUE; // allow player take damage
				}
			}
			// checks if current position of platform reached the original start point 
			if (leftrightspikes[i].x <= leftrightspikes[i].start_x) {
				leftrightspikes[i].pos = OG;
			}
		}
	}
	
}

// Collision code
void platform_collision(s32 cnt, f32 x, f32 y) {
	// Player collision
	for (f32 i = 0; i < cnt; i++) {
		if ((player.center.y <= BLOCK_HEIGHT + y + player.dimensions.y / 2) &&
			(player.center.y >= y + player.dimensions.y / 2) &&
			(player.center.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * cnt + x - player.dimensions.x / 2) &&
			(player.center.x >= x + BLOCK_WIDTH / 2 - player.dimensions.x / 2))
			player.center.y = BLOCK_HEIGHT + y + player.dimensions.y / 2;
	}

	// Enemy1 collision
	for (s32 j = 0; j < MAX_ENEMIES_1; ++j) {
		for (f32 i = 0; i < cnt; i++) {
			if ((enemy1[j].center.y <= BLOCK_HEIGHT + y + enemy1[j].dimensions.y / 2) &&
				(enemy1[j].center.y >= y + enemy1[j].dimensions.y / 2) &&
				(enemy1[j].center.x <= BLOCK_WIDTH / 2 + BLOCK_WIDTH * cnt + x - enemy1[j].dimensions.x / 2) &&
				(enemy1[j].center.x >= x + BLOCK_WIDTH / 2 - enemy1[j].dimensions.x / 2))
				enemy1[j].center.y = BLOCK_HEIGHT + y + enemy1[j].dimensions.y / 2;
		}
	}
}


