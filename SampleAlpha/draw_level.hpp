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

//==== Platform Parameter ====
#define BLOCK_WIDTH 50.0f
#define BLOCK_HEIGHT 50.0f
#define GRAVITY 5.0f

//=== Max Num of Platforms ===
#define MAX_NORMAL 16
#define MAX_LEFT_RIGHT 5
#define MAX_UP_DOWN 3
#define MAX_DIAGONAL_UP 4
#define MAX_DIAGONAL_DOWN 1
#define MAX_ONE_TIME_USE 6
#define MAX_VERTICAL_WALL 1
#define MAX_SPIKES 1
#define MAX_LEFT_RIGHT_SPIKES 2


//======= Flags =======
enum Pos { OG, MOVED };
enum Flag {	ACTIVE,	NOT_ACTIVE };

//---------------------------------------
// Struct for drawing blocks
// --------------------------------------
struct Block {
	AEMtx33 scale{}, rotate{}, translate{}, transform{};
	s32 length;
	f32 x, y = 0.f;
	int pos = OG;
	f32 width, height;
	AEVec2 center{};

	f32 end_x, end_y;
	f32 start_x, start_y;

	int flag = NOT_ACTIVE;
	f64 timer{};
	AEGfxTexture* texture;
};


// ===== Main Functions =====
void draw_level_load();
void draw_level_init();
void draw_level_draw();
void draw_level_update();
void draw_level_free();
void draw_level_unload();

// ===== Create different platform types =====
void normal_blocks_create(s32 len, f32 x, f32 y, s32 index);
void leftright_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index);
void updown_create(s32 len, f32 x, f32 y, f32 start_y, f32 end_y, s32 index);
void diagonal_down_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index);
void diagonal_up_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, f32 start_y, f32 end_y, s32 index);
void one_time_use_create(s32 len, f32 x, f32 y, s32 index);
void vertical_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index);
void spikes_create(s32 len, f32 x, f32 y, s32 index);
void left_right_spikes_create(s32 len, f32 x, f32 y, f32 start_x, f32 end_x, s32 index);


void blocks_draw();
void spikes_draw();
void leftright_blocks_draw();
void updown_blocks_draw();
void diag_up_blocks_draw();
void diag_down_blocks_draw();
void one_time_use_blocks_draw();
void vertical_blocks_draw();
void left_right_spikes_draw();


void left_right_blades(s32 length, f32 x, f32 y);

void move_update();
void platform_collision(s32 cnt, f32 x, f32 y);
void verti_collision(s32 cnt, f32 x, f32 y);
//void trap_collision(s32 cnt, f32 x, f32 y);

void anti_gravity_zone(f32 x1, f32 x2);







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