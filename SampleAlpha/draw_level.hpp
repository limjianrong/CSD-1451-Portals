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

void draw_level_init();
void draw_level();
void update_level();

void blocks(s32 length, f32 x, f32 y);
int check_player_in_gravity_zone(Player_stats player);

#define BLOCK_WIDTH 50.0f
#define BLOCK_HEIGHT 50.0f
#define GRAVITY 5.0f