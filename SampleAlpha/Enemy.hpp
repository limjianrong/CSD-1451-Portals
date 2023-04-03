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
static f32 ENEMY1_WIDTH;				// Enemy1 width
static f32 ENEMY1_HEIGHT;				// Enemy1 height
static f32 ENEMY1_DROPPED_XP;			// Amount of XP player gained when Enemy1 is defeated
static f32 MAX_FRAME_MOVEMENT;			// Max movement value before resetting
static f32 CHANGE_FRAME_MOVEMENT;		// Change direction upon reaching this value
static f32 ENEMY1_MOVEMENTX;			// Amount of X unit movement
static s32 HP_RESET_1;					// HP to reset to when game restarts
static s32 MAX_HP_RESET_1;				// Max HP to reset to when game restarts
static f64 MOVEMENTCOUNTER_RESET;		// Movement counter to reset to when game restarts
static f32 Range_x;						// X Range of bullet delay
static f32 Range_y;						// Y Range of bullet delay
#define MAX_ENEMIES_1 2					// Change this for total number of enemy1

// ----- Enemy 2 (Shoots bullet) -----
static f32 ENEMY2_WIDTH;					// Enemy2 width
static f32 ENEMY2_HEIGHT;					// Enemy2 height
static f32 ENEMY2_TIMER;					// Timer between bullets
static f32 ENEMY2_DROPPED_XP;				// Amount of XP player gained when Enemy2 is defeated
static s32 HP_RESET_2;						// HP to reset to when game restarts
static s32 MAX_HP_RESET_2;					// Max HP to reset to when game restarts
static f64 MAX_MOVEMENT;					// Max movement value before resetting
static f64 CHANGE_MOVEMENT;					// Change direction upon reaching this value
static f32 ENEMY2_MOVEMENTY;				// Amount of Y unit movement
static f32 RangeToEnableBulletDelay;		// Range from enemy2 to player to enable bullet delay 
static f32 Bullet_Displacement_PerFrame;	// Distance travel by bullet every frame
#define MAX_ENEMIES_2 5						// Change this for total number of enemy2

// ----- Bulletss -----
static f32 BULLET_WIDTH;
static f32 BULLET_HEIGHT;
static f32 BULLET_SPEED;


// ------- Enemy specific attributes -------
struct Enemy1_stats : GameObjects {

	f64 movementCounter;
	bool isDamageAllowed;

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



