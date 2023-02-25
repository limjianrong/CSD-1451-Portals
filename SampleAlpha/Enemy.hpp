/*!**************************************************************************************************
\file     Enemy.hpp
\author   Lee Zhi Yee
\par      DP email: zhiyee.l@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file declares the functions used to initialize and update enemy.

  The functions include:
  - enemy_init
	Load texture, initialise mesh

  - draw_enemy
	Draws enemy if its HP is more than 0

  - update_enemy
	Returns the updated position of enemy
*****************************************************************************************************/
#pragma once

#include "AEEngine.h"

#define ENEMY1_HEIGHT 80.f
#define ENEMY1_WIDTH 40.f

struct Enemy1_stats {
	f32 x{}, y{}, rotation{ 0.0f };
	s32 Hp{ 5 };
	AEVec2 center{ 0,0 };
};

void enemy_init();
void draw_enemy();
f32 enemy_update(f32 enemy_x);
//void enemy_collision(); zh
void enemy_collision(Player_stats* player);

/* zh
struct Enemy1_stats {
	f32 x{}, y{}, rotation{0.0f};
	s32 Hp{ 5 };
	//AEVec2 center{ 0,0 }; zh
};
*/


