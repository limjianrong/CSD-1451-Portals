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
#define MAX_ENEMIES_1 2			// Change this for total number of enemy1

// ----- Enemy 2 (Shoots bullet) -----
#define ENEMY2_WIDTH 80.f
#define ENEMY2_HEIGHT 60.f
#define MAX_ENEMIES_2 7			// Change this for total number of enemy2

// ------- Enemy types -------
struct Enemy1_stats {
	f32 x{}, y{}, rotation{ PI };
	f32 width{ ENEMY1_WIDTH }, height{ ENEMY1_HEIGHT };
	s32 Hp{ 5 };
	bool status{ TRUE }; // TRUE for alive, FALSE for dead
	AEVec2 center;

	// ---- Draw ----
	AEMtx33 scale, rotate, translate, transform; // TRS
	AEGfxTexture* texture;

};

struct Enemy2_stats {
	f32 x{}, y{}, rotation{ PI };
	f32 width{ ENEMY2_WIDTH }, height{ ENEMY2_HEIGHT };
	f32 range_x{ ENEMY2_WIDTH + 500 }, range_y{ ENEMY2_HEIGHT + 500 };
	s32 Hp{ 5 };
	bool status{ TRUE }; // TRUE for alive, FALSE for dead
	AEVec2 center;

	// ---- Draw ----
	AEMtx33 scale, rotate, translate, transform; // TRS
	AEGfxTexture* enemy2_fly1, * enemy2_fly2, * enemy2_dead, * bullet;
};

// ------- Enemy1 -------
void enemy1_create(f32 x, f32 y, s32 index);
void draw_enemy1();
//void enemy1_collision(Enemy1_stats enemy1);
void update_enemy1();

// ------- Enemy2 -------
void enemy2_create(f32 x, f32 y, s32 index);
void enemy2_draw();
void enemy2_update();

// ------- Main functions -------
void enemies_load();
void enemies_init();
void enemies_draw();
void enemies_update();
void enemies_unload();
void enemies_free();



