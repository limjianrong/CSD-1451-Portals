/******************************************************************************//*!
\file		draw_level.hpp
\author 	Digipen, Tay Zhun Hang
\par    	email: zhunhang.tay@digipen.edu
\date   	12 February, 2023
\brief

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#pragma once
#include "Player.hpp"
#include "Utilities.hpp"
#include <vector>

#define BLOCK_WIDTH 50.0f
#define BLOCK_HEIGHT 50.0f
#define GRAVITY 5.0f
#define NUM_OF_MOVING_PLAT 1
#define SHORT_DIST 450.0f
#define MID_DIST 650.0f

enum { OG, MOVED };
enum Flag {
	ACTIVE,
	NOT_ACTIVE
};

//---------------------------------------
// Struct for drawing blocks
// --------------------------------------
struct Block {
	//sstring name;
	AEMtx33 scale{}, rotate{}, translate{}, transform{};
	s32 length;
	f32 x, y = 0.f;
	int pos = OG;
	bool isStatic{ false };
	f32 width, height;
	//AEGfxVertexList* mesh{}; //= create_Square_Mesh();
	AEVec2 center{};

	f32 end_x, end_y;
	f32 start_x, start_y; //for oscillating behaviour

	int flag = NOT_ACTIVE;
	f64 timer = 0.f;
};
//std::vector<Block> blocklist;

// ---- Main Functions ----
void draw_level_load();
void draw_level_init();
void draw_level();
void update_level();

void blocks(s32 length, f32 x, f32 y);
void spikes(s32 length, f32 x, f32 y);
void leftright_blocks(s32 length, f32 x, f32 y);
void updown_blocks(s32 length, f32 x, f32 y);
void diag_up_blocks(s32 length, f32 x, f32 y);
void diag_down_blocks(s32 length, f32 x, f32 y);
void one_use_blocks(s32 length, f32 x, f32 y);
void verti_blocks(s32 length, f32 x, f32 y);
void dropping_spikes(s32 length, f32 x, f32 y);
void left_right_blades(s32 length, f32 x, f32 y);

void move_update();
void platform_collision(s32 cnt, f32 x, f32 y);
void verti_collision(s32 cnt, f32 x, f32 y);
void trap_collision(s32 cnt, f32 x, f32 y);
void anti_gravity_zone(f64 x1, f64 x2);
void draw_slippery_platform();
void update_slippery_platform();


struct Slippery_platform {
	f32 x{}, y{};
	AEVec2 center{ 0,0 };
	f32 width{ 200 }, height{ 100 };
	AEMtx33 transform{};
	bool status{};
};
//note to myself
//create a array of struct
//create a function called create_moving_platform
//	- provide the variables to be put into the struct
// make min max to be +/- of the variables
// use define num of moving platforms, num of platforms, traps as the size of array
// 
// can try use vector
//
//
//