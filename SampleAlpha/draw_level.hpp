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
#include <vector>

#define BLOCK_WIDTH 50.0f
#define BLOCK_HEIGHT 50.0f
#define GRAVITY 5.0f
#define NUM_OF_MOVING_PLAT 1
#define SHORT_DIST 450.0f
#define MID_DIST 650.0f

enum { OG, MOVED };

//std::vector<Block> blocklist;

void draw_level_init();
void draw_level();
void update_level();

void blocks(s32 length, f32 x, f32 y);
int check_player_in_gravity_zone(Player_stats player);
void spikes(s32 length, f32 x, f32 y);
void leftright_blocks(s32 length, f32 x, f32 y);
void updown_blocks(s32 length, f32 x, f32 y);
void move_update();
void platform_collision(s32 cnt, f32 x, f32 y);
void trap_collision(s32 cnt, f32 x, f32 y);
struct Block {
	//sstring name;
	AEMtx33 scale{}, rotate{}, translate{}, transform{};
	s32 length;
	f32 x, y = 0.f;
	int pos = OG;
	bool isStatic{ false };
	f32 width, height;
	AEGfxVertexList* mesh{};
	AEVec2 center{};

	f32 max_x, max_y;
	f32 min_x, min_y; //for oscillating behaviour
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