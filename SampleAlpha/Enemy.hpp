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

// ----- Enemy 1 -----
#define ENEMY1_WIDTH 60.f
#define ENEMY1_HEIGHT 80.f

// ----- Enemy 2 (Shoots bullet) -----
#define ENEMY2_WIDTH 80.f
#define ENEMY2_HEIGHT 60.f

// ------- Enemy types -------
struct Enemy1_stats {
	f32 x{}, y{}, rotation{ PI };
	f32 width{ ENEMY1_WIDTH }, height{ ENEMY1_HEIGHT };
	s32 Hp{ 5 };
	//AEVec2 center;
	AEGfxTexture* texture;
};

struct Enemy2_stats {
	f32 x{}, y{}, rotation{ PI };
	f32 width{ ENEMY2_WIDTH }, height{ ENEMY2_HEIGHT };
	f32 range_x{ ENEMY2_WIDTH + 200 }, range_y{ ENEMY2_HEIGHT + 200 };
	s32 Hp{ 5 };
	AEVec2 center;

	// ----- Texture -----
	AEGfxTexture* enemy2_fly1, * enemy2_fly2, *enemy2_dead;
};

void enemy_init();
void draw_enemy1(Enemy1_stats enemy1);
void draw_enemy();
void update_enemy();
void unload_enemy();
//void enemy_collision(); zh
void enemy1_collision(Enemy1_stats enemy1);



