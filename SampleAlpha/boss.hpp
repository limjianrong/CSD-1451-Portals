/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:							Boss.hpp
* Course:						CSD1451
* Group Name:					Apparate
* Primary Author (90%):			Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors (10%):		Lin ZhaoZhi (z.lin@digipen.edu)
*
* Brief:
  This source file implements the functions used for the boss attacks 1,2 and 3. As well
  as functions used for boss movement and teleportation.
==================================================================================*/
#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "GameState_Platformer.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "AEEngine.h"
#include "DrawLevel.hpp"

#define BOSS_ZONE_RANGE 1500.f

// COMMAND ENUMS
enum { UP, DOWN, LEFT, RIGHT, STOP };

//variables of the boss
struct Boss : GameObjects {

	//range of boss's bullet
	f32 range_x, range_y; 
	s32 direction{ UP };
	f32 velocity;

	// ---- Texture ----
	AEMtx33 scale, translate, matrix{};
	AEGfxTexture* standTex, * deadTex;

};


// ------ Attack #1 ------
//variables of the boss's laser beam attack
struct Laser_beam{
	
	//laser beam attack is active or not
	bool active;

	//position, width, height, cooldown of laser beam attack
	f32 x, y;
	f32 width, height, cooldown;

	//duration refers to how long the laser beam attack has been active
	//buffer duration is used to warn the player that the boss is about
	//to commence the laser beam attack
	f32 duration, buffer_duration;

	//duration of the laser beam attack
	f32 max_duration;

	//time elapsed since the previous laser beam attack
	f32 time_elapsed;

	//sets the flag if the player has been damaged by the laser beam attack
	bool damaged_player;

	//center of laser beam
	AEVec2 center;

	// ---- Texture ----
	AEMtx33 scale, translate, rotate, final_matrix;
	AEGfxTexture* picture;

	//variables  used to let players know that
	//the boss is about to start the laser beam attack
	AEGfxTexture* warning_pic;
	f32 warning_pic_width, warning_pic_height;
	AEMtx33 warning_pic_scale, warning_pic_translate, warning_pic_matrix;
};

// ------ Attack #2 ------
#define BOSS_TIMER 0.5f
#define DISTANCE_BEFORE_BULLET_DISAPPEAR 600.f
struct Bullet {
	f32 x, y;
	AEVec2 center, direction;
	f32 width, height;
	f32 speed, timer;
	bool isTimerActive, isTeleported, isShooting, isReset;

	// ----- Mesh & Texture -----
	AEGfxTexture* bulletTex;
};

// ------ Attack #3  -------
//variables of the boss's charge attack
struct Boss_charge {
	//flags used to at different parts of the boss charge attack
	bool active, return_to_position, damaged_player;

	//cooldown of the attack, range of the attack, speed of the
	//boss during its attack, as well as time elapsed since
	//the previous charge attack
	f32 cooldown, range, velocity, time_elapsed;

	//distance travelled by boss during charge attack
	f32 distance_travelled, direction_magnitude;

	//direction of the boss prior to its charge attack
	
	s32 previous_direction;


	//original position when boss first starts the charge attack
	AEVec2 original_position;

	//direction of boss's charge
	AEVec2 direction;

	//normalized vector of the boss's direction to charge towards
	AEVec2 normalized_direction;

	//endpoint of boss's charge
	AEVec2 endpoint;

};

//boss teleportation, cooldown, time elapsed since last teleport
//location of where to teleport to
struct Boss_teleport {
	f32 cooldown, time_elapsed;
	AEVec2 location;
};
// ---- Main Functions ----

//loads the assets used by the boss
void boss_load();

//initializes some variables used by the boss
void boss_init();

//draws the boss
void boss_draw();

//updates boss movement and boss attacks
void boss_update();

//frees any objects related to the boss
void boss_free();

//unloads assets used by the boss
void boss_unload();

//boss movement
void boss_movement();

//boss teleportation
void boss_movement_teleport();

//boss attack #1
void boss_laser_beam_attack();

//boss attack #2
void bullet_update();

//boss attack #3
void boss_charge_attack();

//function to draw the boss's laser beam
void draw_laser_beam();

//function to draw the boss's laser beam warning, before the actual laser beam attack
void draw_laser_beam_warning();

//draws the boss bullets
void bullet_draw();
