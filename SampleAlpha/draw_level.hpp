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
#define BLOCK_WIDTH 50.0f
#define BLOCK_HEIGHT 50.0f
#define GRAVITY 5.0f
#define NUM_OF_MOVING_PLAT 1

void draw_level_init();
void draw_level();
void update_level();

void blocks(s32 length, f32 x, f32 y);
int check_player_in_gravity_zone(Player_stats player);
void traps(s32 length, f32 x, f32 y);
void moving_blocks(s32 length, f32 x, f32 y);
void move_update();
//f32 move_update(f32 x);

enum{OG, MOVED};

struct move_block {
	AEMtx33 scale{}, rotate{}, translate{}, transform{};
	s32 length;
	f32 x, y;
	int pos = OG;
	f32 width, height;
	AEGfxVertexList* mesh{};
	AEVec2 center{};
} ;

