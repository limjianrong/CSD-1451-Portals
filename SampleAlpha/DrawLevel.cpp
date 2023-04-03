/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					DrawLevel.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Tay Zhun Hang (zhunhang.tay@digipen.edu)
*
* Brief:
  This source file implements the functions used for the level design of the game. It
  also includes the source code for the logic of different platforms types.
==================================================================================*/
#include "DrawLevel.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

// --- Externs ---
extern Player_stats player;
extern Enemy1_stats enemy1[MAX_ENEMIES_1];
extern AEAudio playerDamageAudio;
extern AEAudioGroup soundGroup;
extern AEGfxVertexList* square_mesh;	// Created square mesh

// --- Structs ---
Block normal[MAX_NORMAL], left_right[MAX_LEFT_RIGHT], up_down[MAX_UP_DOWN], diagonal_up[MAX_DIAGONAL_UP],
diagonal_down[MAX_DIAGONAL_DOWN], one_time_use[MAX_ONE_TIME_USE], floor_spikes[MAX_SPIKES], left_right_spikes[MAX_LEFT_RIGHT_SPIKES];
Door door;

// --- Variables ---
AEGfxTexture* platform_text, * spike_text, * onetime_text;
AEMtx33 scale, rotate, translate, transform; // TRS
bool damage_ok{ TRUE };

// --- File IO ---
std::ifstream door_ifs{};

/******************************************************************************/
//
//	Load function for this game state.
//
/******************************************************************************/
void draw_level_load() {
	// Load Textures
	platform_text	= AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile001.png");
	onetime_text	= AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile013.png");
	spike_text		= AEGfxTextureLoad("Assets/abstract-platformer/PNG/Other/spikesHigh.png");
	door.picture	= AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile052.png");
	
	// Load door variables from std::ifstream
	door_ifs.open("Assets/textFiles/door.txt");
	std::string str{};
	door_ifs >> str >> door.center.x;
	door_ifs >> str >> door.center.y;
	door_ifs >> str >> door.width;
	door_ifs >> str >> door.height;
}


/******************************************************************************/
//
//	Initialisation function for this game state. Initializes all the variables
//	for the different platforms and spikes
//
/******************************************************************************/
void draw_level_init() {

	// Spikes
	spikes_create(2, 1550, 150, 0);

	// Moving Left And Right Spikes
	left_right_spikes_create(1, 4800, 525, 4800, 5450, 0);
	left_right_spikes_create(1, 4950, 525, 4950, 5600, 1);

	// Normal Platforms (Top Route)
	normal_create(6, -500, -200, 0);
	normal_create(5, 425, 0, 1);
	normal_create(5, 900, 350, 2);
	normal_create(5, 1400, 125, 3);
	normal_create(5, 2250, 500, 4);
	normal_create(6, 3450, 850, 5);
	normal_create(7, 5400, 950, 6);
	normal_create(7, 6550, 1000, 7);
	normal_create(4, 7000, 750, 8);

	// Normal Platforms (Bottom Route)
	normal_create(4, 800, -150, 9);
	normal_create(4, 2550, -250, 10);
	normal_create(6, 3350, 200, 11);
	normal_create(5, 4200, 300, 12);
	normal_create(4, 5800, 100, 13);
	normal_create(7, 6900, 300, 14);

	// Normal Platforms (Boss Area)
	normal_create(20, 7600, 450, 15); 
	normal_create(7, 7300, 200, 16);
	normal_create(7, 7300, 625, 17);

	// Moving Left And Right Platforms (Top Route)
	left_right_create(4, -150, -100, -150, 200, 0);
	left_right_create(5, 5850, 950, 5850, 6250, 1);

	// Moving Left And Right Platforms (Bottom Route)	
	left_right_create(4, 1050, -200, 1050, 1850, 2);
	left_right_create(4, 4800, 500, 4800, 5450, 3);
	left_right_create(4, 6150, 200, 6150, 6650, 4);
	
	// Moving Up And Down Platforms (Top Route)
	up_down_create(3, 700, 125, 125, 325, 0);
	up_down_create(3, 2550, 600, 600, 775, 1);

	// Moving Up And Down Platforms (Bottom Route)
	up_down_create(3, 4550, 400, 400, 650, 2);

	// Moving Diagonally Upward Platforms (Top Route)
	diagonal_up_create(4, 1700, 150, 1700, 2000, 150, 450, 0);
	diagonal_up_create(4, 3800, 900, 3800, 4100, 900, 1200, 1);
	
	// Moving Diagonally Upward Platforms (Bottom Route)
	diagonal_up_create(4, 2000, -150, 2000, 2300, -150, 150, 2);
	diagonal_up_create(3, 2800, -200, 2800, 3150, -200, 150, 3);

	// Moving Diagonally Downward Platform
	diagonal_down_create(4, 4700, 1200, 4700, 5050, 1200, 850, 0);

	// Disappearing platforms (Top Route)
	one_time_use_create(4, 2750, 850, 0);
	one_time_use_create(4, 3100, 850, 1);
	one_time_use_create(4, 4400, 1300, 2);

	// Disappearing platforms (Bottom Route)
	one_time_use_create(4, 2550, 175, 3);
	one_time_use_create(4, 3800, 300, 4);
	one_time_use_create(4, 5800, 500, 5);

	// Initialize Disappearing Platforms' flag
	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		one_time_use[i].flag = ACTIVE;
	}
}

/******************************************************************************/
//
//	Draw function for this game state. Draw all platforms and spikes
//
/******************************************************************************/
void draw_level_draw() {
	spikes_draw();
	left_right_spikes_draw();
	blocks_draw();
	left_right_blocks_draw();
	up_down_blocks_draw();
	diagonal_up_blocks_draw();
	diagonal_down_blocks_draw();
	one_time_use_blocks_draw();
	door_draw();
}

// -------- Normal Blocks Create Function --------
void normal_create(s32 len, f32 x, f32 y, s32 index) {
	normal[index].length				= len;
	normal[index].x						= x;
	normal[index].y						= y;
}

// -------- Moving Left And Right Blocks Create Function --------
void left_right_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index) {
	left_right[index].length			= len;
	left_right[index].x					= x;
	left_right[index].y					= y;
	left_right[index].start_x			= start_x;
	left_right[index].end_x				= end_x;
}

// -------- Moving Up And Down Blocks Create Function --------
void up_down_create(s32 len, f32 x, f32 y, f32 start_y, f32 end_y, s32 index) {
	up_down[index].length				= len;
	up_down[index].x					= x;
	up_down[index].y					= y;
	up_down[index].start_y				= start_y;
	up_down[index].end_y				= end_y;
}

// -------- Moving Diagonally Upward Blocks Create Function --------
void diagonal_up_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index) {
	diagonal_up[index].length			= len;
	diagonal_up[index].x				= x;
	diagonal_up[index].y				= y;
	diagonal_up[index].start_x			= start_x;
	diagonal_up[index].end_x			= end_x;
	diagonal_up[index].start_y			= start_y;
	diagonal_up[index].end_y			= end_y;
}

// -------- Moving Diagonally Downward Blocks Create Function --------
void diagonal_down_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index) {
	diagonal_down[index].length			= len;
	diagonal_down[index].x				= x;
	diagonal_down[index].y				= y;
	diagonal_down[index].start_x		= start_x;
	diagonal_down[index].end_x			= end_x;
	diagonal_down[index].start_y		= start_y;
	diagonal_down[index].end_y			= end_y;
}

// -------- Disappearing Blocks Create Function --------
void one_time_use_create(s32 len, f32 x, f32 y, s32 index) {
	one_time_use[index].length			= len;
	one_time_use[index].x				= x;
	one_time_use[index].y				= y;
}

// -------- Spikes Create Function --------
void spikes_create(s32 len, f32 x, f32 y, s32 index) {
	floor_spikes[index].length			= len;
	floor_spikes[index].x				= x;
	floor_spikes[index].y				= y;
}

// -------- Moving Left And Right Spikes Create Function --------
void left_right_spikes_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index) {
	left_right_spikes[index].length		= len;
	left_right_spikes[index].x			= x;
	left_right_spikes[index].y			= y;
	left_right_spikes[index].start_x	= start_x;
	left_right_spikes[index].end_x		= end_x;
}

/******************************************************************************/
//
//	Update function for this game state. Contains the code for gravity
//  and the different platforms' logic
//
/******************************************************************************/
void draw_level_update() {

	// Gravity code for player
	player.center.y -= GRAVITY;

	// Gravity code for enemy1
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		enemy1[i].center.y -= GRAVITY;
	}

	// -------- Moving Left And Right Platform Logic --------
	for (s32 i = 0; i < MAX_LEFT_RIGHT; i++) {

		//checks if the platform is at its original position
		if (left_right[i].pos == OG) {
			left_right[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves platform

			// checks if player is standing on the platform
			if (AETestRectToRect(&left_right[i].center, BLOCK_WIDTH * left_right[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

				// moves player together with platform
				player.center.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}

			// checks if current position of platform reached the desired end point 
			if (left_right[i].x >= left_right[i].end_x) {
				left_right[i].pos = MOVED; // set platform position to MOVED status
			}
		}

		//checks if the platform is at its moved positoin
		if (left_right[i].pos == MOVED) {
			left_right[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves platform

			// checks if player is standing on the platform
			if (AETestRectToRect(&left_right[i].center, BLOCK_WIDTH * left_right[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

				// moves player together with platform
				player.center.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; 
			}

			// checks if current position of platform reached the original start point 
			if (left_right[i].x <= left_right[i].start_x) {
				left_right[i].pos = OG; // set platform position to Original status
			}
		}
	}

	// -------- Moving Up And Down Platform Logic --------
	for (s32 i = 0; i < MAX_UP_DOWN; i++) {

		// checks if the platform is at its original position
		if (up_down[i].pos == OG) {
			up_down[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;  // moves platform

			// checks if player is standing on the platform
			if (AETestRectToRect(&up_down[i].center, BLOCK_WIDTH * up_down[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

				// moves player together with platform
				player.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}

			// checks if current position of platform reached the desired end point 
			if (up_down[i].y >= up_down[i].end_y) {
				up_down[i].pos = MOVED;
			}
		}
		
		// checks if the platform is at its moved positoin
		if (up_down[i].pos == MOVED) {
			up_down[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;  // moves platform

			// checks if player is standing on the platform
			if (AETestRectToRect(&up_down[i].center, BLOCK_WIDTH * up_down[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y))

				// moves player together with platform
				player.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; 
			
			// checks if current position of platform reached the original start point 
			if (up_down[i].y <= up_down[i].start_y) {
				up_down[i].pos = OG;
			}
		}
	}

	// -------- Moving Diagonally Upward Platform Logic --------
	for (s32 i = 0; i < MAX_DIAGONAL_UP; i++) {

		// checks if the platform is at its original position
		if (diagonal_up[i].pos == OG) {
			// moves platform
			diagonal_up[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonal_up[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;

			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonal_up[i].center, BLOCK_WIDTH * diagonal_up[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

				// moves player together with platform
				player.center.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}

			// checks if current position of platform reached the desired end point 
			if (diagonal_up[i].y >= diagonal_up[i].end_y) {
				diagonal_up[i].pos = MOVED;
			}
		}

		// checks if the platform is at its moved positoin
		if (diagonal_up[i].pos == MOVED) {

			// moves platform
			diagonal_up[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonal_up[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;

			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonal_up[i].center, BLOCK_WIDTH * diagonal_up[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

				// moves player together with platform
				player.center.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}

			// checks if current position of platform reached the original start point 
			if (diagonal_up[i].y <= diagonal_up[i].start_y) {
				diagonal_up[i].pos = OG;
			}
		}
	}

	// -------- Moving Diagonally Downward Platform Logic --------
	for (s32 i = 0; i < MAX_DIAGONAL_DOWN; i++) {
		// checks if the platform is at its original position
		if (diagonal_down[i].pos == OG) {
			// moves platform
			diagonal_down[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonal_down[i].y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;

			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonal_down[i].center, BLOCK_WIDTH * diagonal_down[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {
				
				// moves player together with platform
				player.center.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}

			// checks if current position of platform reached the desired end point 
			if (diagonal_down[i].y <= diagonal_down[i].end_y) {//&& diagdown1.x >= diagdown1.end_x
				diagonal_down[i].pos = MOVED;
			}
		}
		// checks if the platform is at its moved positoin
		if (diagonal_down[i].pos == MOVED) {

			// moves platform
			diagonal_down[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			diagonal_down[i].y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;

			// checks if player is standing on the platform
			if (AETestRectToRect(&diagonal_down[i].center, BLOCK_WIDTH * diagonal_down[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

				// moves player together with platform
				player.center.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
				player.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
			}

			// checks if current position of platform reached the original start point 
			if (diagonal_down[i].y >= diagonal_down[i].start_y) {
				diagonal_down[i].pos = OG;
			}
		}
	}

	// -------- Disappearing Platform Logic --------
	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {

		// checks if player is standing on the platform
		if (AETestRectToRect(&one_time_use[i].center, BLOCK_WIDTH * one_time_use[i].length, 
			BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

			// starts the timer to despawn the platform
			one_time_use[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime()); 

			// checks if 3 seconds or more after player stepped on platform
			if (one_time_use[i].timer >= 3) {
				if (one_time_use[i].flag == ACTIVE) {
					// set flag for platform to not active
					one_time_use[i].flag = NOT_ACTIVE;
					one_time_use[i].timer = 0; // resets timer
				}
			}
		}
		else if (one_time_use[i].timer > 0) { // checks if timer has started
			one_time_use[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime()); //continue the timer

			// checks if 3 seconds or more after player stepped on platform
			if (one_time_use[i].timer >= 3) {
				if (one_time_use[i].flag == ACTIVE) {
					// set flag for platform to not active
					one_time_use[i].flag = NOT_ACTIVE;
					one_time_use[i].timer = 0; // resets timer
				}
			}
		}

		// checks if platform flag is no longer active
		if (one_time_use[i].flag == NOT_ACTIVE) {

			// starts the timer to respawn the platform
			one_time_use[i].timer += static_cast<f32>(AEFrameRateControllerGetFrameTime());
			if (one_time_use[i].timer >= 3) {
				one_time_use[i].timer = 0;
				one_time_use[i].flag = ACTIVE; // respawns the platform
			}
		}
	}

	// -------- Spikes Logic --------
	for (s32 i = 0; i < MAX_SPIKES; i++) {
		// checks if player is standing in the spikes
		if (AETestRectToRect(&floor_spikes[i].center, BLOCK_WIDTH * floor_spikes[i].length, 
			BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

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

	// -------- Moving Left And Right Spikes Logic --------
	for (s32 i = 0; i < MAX_LEFT_RIGHT_SPIKES; i++) {

		// checks if the spikes is at its original position
		if (left_right_spikes[i].pos == OG) {
			left_right_spikes[i].x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves spikes

			// check if player is standing on the spikes
			if (AETestRectToRect(&left_right_spikes[i].center, BLOCK_WIDTH * left_right_spikes[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

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
			if (left_right_spikes[i].x >= left_right_spikes[i].end_x) {
				left_right_spikes[i].pos = MOVED;
			}
		}

		// checks if the spikes is at its moved positoin
		if (left_right_spikes[i].pos == MOVED) {
			left_right_spikes[i].x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED; // moves spikes

			// check if player is standing on the spikes
			if (AETestRectToRect(&left_right_spikes[i].center, BLOCK_WIDTH * left_right_spikes[i].length, 
				BLOCK_HEIGHT * 2, &player.center, player.dimensions.x, player.dimensions.y)) {

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
			if (left_right_spikes[i].x <= left_right_spikes[i].start_x) {
				left_right_spikes[i].pos = OG;
			}
		}
	}
}

/******************************************************************************/
//
//	Free function for this game state.
//
/******************************************************************************/
void draw_level_free() {

}

/******************************************************************************/
//
//	Unload function for this game state. Unloads all textures and free meshes
//
/******************************************************************************/
void draw_level_unload() {
	// Texture unload
	AEGfxTextureUnload(platform_text);
	AEGfxTextureUnload(spike_text);
	AEGfxTextureUnload(onetime_text);
	AEGfxTextureUnload(door.picture);
}

// -------- Normal Platform Draw Function --------
void blocks_draw() {
	for (s32 i = 0; i < MAX_NORMAL; i++) {
		for (s32 j = 0; j < normal[i].length; j++) {
			AEMtx33Scale(&scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&rotate, NULL);
			AEMtx33Trans(&translate, BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + normal[i].x
				, BLOCK_HEIGHT / 2 + normal[i].y);
			// Concat the matrices
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

// -------- Spikes Draw Function --------
void spikes_draw() {
	for (s32 i = 0; i < MAX_SPIKES; i++) {
		for (s32 j = 0; j < floor_spikes[i].length; j++) {
			floor_spikes[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + floor_spikes[i].x;
			floor_spikes[i].height = BLOCK_HEIGHT / 2 + floor_spikes[i].y;

			AEMtx33Scale(&floor_spikes[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&floor_spikes[i].rotate, NULL);
			AEMtx33Trans(&floor_spikes[i].translate, floor_spikes[i].width, floor_spikes[i].height);
			// Concat the matrices
			AEMtx33Concat(&floor_spikes[i].transform, &floor_spikes[i].rotate, &floor_spikes[i].scale);
			AEMtx33Concat(&floor_spikes[i].transform, &floor_spikes[i].translate, &floor_spikes[i].transform);
			AEGfxSetTransform(floor_spikes[i].transform.m);
			// Set the texture
			AEGfxTextureSet(spike_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		}
		// Set the center
		AEVec2Set(&floor_spikes[i].center, (BLOCK_WIDTH * floor_spikes[i].length) / 2 + floor_spikes[i].x, 
			floor_spikes[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// -------- Moving Left & Right Platform Draw Function --------
void left_right_blocks_draw() {

	for (s32 i = 0; i < MAX_LEFT_RIGHT; i++) {
		for (s32 j = 0; j < left_right[i].length; j++) {
			left_right[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + left_right[i].x;
			left_right[i].height = BLOCK_HEIGHT / 2 + left_right[i].y;

			AEMtx33Scale(&left_right[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&left_right[i].rotate, NULL);
			AEMtx33Trans(&left_right[i].translate, left_right[i].width, left_right[i].height);
			// Concat the matrices
			AEMtx33Concat(&left_right[i].transform, &left_right[i].rotate, &left_right[i].scale);
			AEMtx33Concat(&left_right[i].transform, &left_right[i].translate, &left_right[i].transform);
			AEGfxSetTransform(left_right[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(left_right[i].length, left_right[i].x, left_right[i].y);
		}
		// Set the center
		AEVec2Set(&left_right[i].center, (BLOCK_WIDTH * left_right[i].length) / 2 + left_right[i].x, 
			left_right[i].y + BLOCK_HEIGHT / 4 * 3);

	}
}

// -------- Moving Up & Down Platform Draw Function --------
void up_down_blocks_draw() {

	for (s32 i = 0; i < MAX_UP_DOWN; i++) {
		for (s32 j = 0; j < up_down[i].length; j++) {
			up_down[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + up_down[i].x;
			up_down[i].height = BLOCK_HEIGHT / 2 + up_down[i].y;

			AEMtx33Scale(&up_down[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&up_down[i].rotate, NULL);
			AEMtx33Trans(&up_down[i].translate, up_down[i].width, up_down[i].height);
			// Concat the matrices
			AEMtx33Concat(&up_down[i].transform, &up_down[i].rotate, &up_down[i].scale);
			AEMtx33Concat(&up_down[i].transform, &up_down[i].translate, &up_down[i].transform);
			AEGfxSetTransform(up_down[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms
			platform_collision(up_down[i].length, up_down[i].x, up_down[i].y);


		}
		AEVec2Set(&up_down[i].center, (BLOCK_WIDTH * up_down[i].length) / 2 + up_down[i].x, up_down[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// -------- Moving Diagonally Upward Platform Draw Function --------
void diagonal_up_blocks_draw() {

	for (s32 i = 0; i < MAX_DIAGONAL_UP; i++) {
		for (s32 j = 0; j < diagonal_up[i].length; j++) {
			diagonal_up[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + diagonal_up[i].x;
			diagonal_up[i].height = BLOCK_HEIGHT / 2 + diagonal_up[i].y;

			AEMtx33Scale(&diagonal_up[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&diagonal_up[i].rotate, NULL);
			AEMtx33Trans(&diagonal_up[i].translate, diagonal_up[i].width, diagonal_up[i].height);
			// Concat the matrices
			AEMtx33Concat(&diagonal_up[i].transform, &diagonal_up[i].rotate, &diagonal_up[i].scale);
			AEMtx33Concat(&diagonal_up[i].transform, &diagonal_up[i].translate, &diagonal_up[i].transform);
			AEGfxSetTransform(diagonal_up[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms
			platform_collision(diagonal_up[i].length, diagonal_up[i].x, diagonal_up[i].y);
		}
		// Set the center
		AEVec2Set(&diagonal_up[i].center, (BLOCK_WIDTH * diagonal_up[i].length) / 2 + diagonal_up[i].x, 
			diagonal_up[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// -------- Moving Diagonally Downward Platform Draw Function --------
void diagonal_down_blocks_draw() {
	for (s32 i = 0; i < MAX_DIAGONAL_DOWN; i++) {
		for (s32 j = 0; j < diagonal_down[i].length; j++) {
			diagonal_down[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + diagonal_down[i].x;
			diagonal_down[i].height = BLOCK_HEIGHT / 2 + diagonal_down[i].y;

			AEMtx33Scale(&diagonal_down[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&diagonal_down[i].rotate, NULL);
			AEMtx33Trans(&diagonal_down[i].translate, diagonal_down[i].width, diagonal_down[i].height);
			// Concat the matrices
			AEMtx33Concat(&diagonal_down[i].transform, &diagonal_down[i].rotate, &diagonal_down[i].scale);
			AEMtx33Concat(&diagonal_down[i].transform, &diagonal_down[i].translate, &diagonal_down[i].transform);
			AEGfxSetTransform(diagonal_down[i].transform.m);

			// Set the texture
			AEGfxTextureSet(platform_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

			// Player collision with platforms

			platform_collision(diagonal_down[i].length, diagonal_down[i].x, diagonal_down[i].y);
		}
		// Set the center
		AEVec2Set(&diagonal_down[i].center, (BLOCK_WIDTH * diagonal_down[i].length) / 2 + diagonal_down[i].x, 
			diagonal_down[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// -------- Disappearing Platform Draw Function --------
void one_time_use_blocks_draw() {

	for (s32 i = 0; i < MAX_ONE_TIME_USE; i++) {
		for (s32 j = 0; j < one_time_use[i].length; j++) {
			one_time_use[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + one_time_use[i].x;
			one_time_use[i].height = BLOCK_HEIGHT / 2 + one_time_use[i].y;

			AEMtx33Scale(&one_time_use[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&one_time_use[i].rotate, NULL);
			AEMtx33Trans(&one_time_use[i].translate, one_time_use[i].width, one_time_use[i].height);
			// Concat the matrices
			AEMtx33Concat(&one_time_use[i].transform, &one_time_use[i].rotate, &one_time_use[i].scale);
			AEMtx33Concat(&one_time_use[i].transform, &one_time_use[i].translate, &one_time_use[i].transform);
			AEGfxSetTransform(one_time_use[i].transform.m);

			// Set the texture
			AEGfxTextureSet(onetime_text, 0, 0);
			if (one_time_use[i].flag == ACTIVE) {
				AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
				// Player collision with platforms
				platform_collision(one_time_use[i].length, one_time_use[i].x, one_time_use[i].y);
			}
		}
		// Set the center
		AEVec2Set(&one_time_use[i].center, (BLOCK_WIDTH * one_time_use[i].length) / 2 + one_time_use[i].x, 
			one_time_use[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// -------- Moving Left And Right Spikes Draw Function --------
void left_right_spikes_draw() {

	for (s32 i = 0; i < MAX_LEFT_RIGHT_SPIKES; i++) {
		for (s32 j = 0; j < left_right_spikes[i].length; j++) {
			left_right_spikes[i].width = BLOCK_WIDTH / 2 + BLOCK_WIDTH * j + left_right_spikes[i].x;
			left_right_spikes[i].height = BLOCK_HEIGHT / 2 + left_right_spikes[i].y;

			AEMtx33Scale(&left_right_spikes[i].scale, BLOCK_WIDTH, BLOCK_HEIGHT);
			AEMtx33Rot(&left_right_spikes[i].rotate, NULL);
			AEMtx33Trans(&left_right_spikes[i].translate, left_right_spikes[i].width, left_right_spikes[i].height);
			// Concat the matrices
			AEMtx33Concat(&left_right_spikes[i].transform, &left_right_spikes[i].rotate, &left_right_spikes[i].scale);
			AEMtx33Concat(&left_right_spikes[i].transform, &left_right_spikes[i].translate, &left_right_spikes[i].transform);
			AEGfxSetTransform(left_right_spikes[i].transform.m);

			// Set the texture
			AEGfxTextureSet(spike_text, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
		AEVec2Set(&left_right_spikes[i].center, (BLOCK_WIDTH * left_right_spikes[i].length) / 2 + left_right_spikes[i].x, 
			left_right_spikes[i].y + BLOCK_HEIGHT / 4 * 3);
	}
}

// -------- Door Draw Function --------
void door_draw() {
	AEMtx33Scale(&door.scale, door.width, door.height);
	AEMtx33Trans(&door.translate, door.center.x, door.center.y);
	AEMtx33Concat(&door.final, &door.translate, &door.scale);
	AEGfxSetTransform(door.final.m);
	AEGfxTextureSet(door.picture, 0.0f, 0.0f);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}

// -------- Collision code --------
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


