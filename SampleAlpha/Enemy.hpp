/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Enemy.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This header file contains all the definitions for functions and derived struct class used for enemy1 and enemy2
  This includes drawing of their mesh with respective texture, movement, collision and related logic
==================================================================================*/
#pragma once

#include "AEEngine.h"
#include "Utilities.hpp"

// ----- Enemy 1 -----
#define ENEMY1_WIDTH 60.f			// Enemy1 width
#define ENEMY1_HEIGHT 80.f			// Enemy1 height
#define ENEMY1_DROPPED_XP 20.f		// Amount of XP player gained when Enemy1 is defeated
#define MAX_ENEMIES_1 3				// Change this for total number of enemy1

// ----- Enemy 2 (Shoots bullet) -----
#define ENEMY2_WIDTH 80.f			// Enemy2 width
#define ENEMY2_HEIGHT 60.f			// Enemy2 height
#define ENEMY2_TIMER 0.75f			// Timer between bullets
#define ENEMY2_DROPPED_XP 40.f		// Amount of XP player gained when Enemy2 is defeated
#define MAX_ENEMIES_2 5				// Change this for total number of enemy2

#define Bullet_Displacement_PerFrame 5  // Distance travel by bullet every frame


// ------- Enemy specific attributes -------
struct Enemy1_stats : GameObjects {

	f64 movementCounter;

	// ---- Draw ----
	AEGfxTexture* texture;

};

struct Enemy2_stats : GameObjects {

	f32 range_x, range_y;
	f64 movingTime;

	// ---- Draw ----
	AEMtx33 scale{}, rotate{}, translate{}, transform{}; // TRS
	AEGfxTexture* enemy2_fly1, * enemy2_fly2, * enemy2_dead, * bullet;
};

// ------- Enemy1 -------
void enemy1_create(f32 x, f32 y, s32 index);
void enemy1_draw();
//void enemy1_collision(Enemy1_stats enemy1);
void enemy1_update();


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



