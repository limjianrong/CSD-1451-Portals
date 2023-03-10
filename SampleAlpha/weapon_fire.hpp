#pragma once
/*!**************************************************************************************************
\file     weapon_fire.hpp
\author   Lin ZhaoZhi
\par      DP email: z.lin@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file implements the functions used to initialize and bullet mechanics.

  The function includes:
  - bullet_initialise
	Load texture, initialise mesh

  - weapon_fire
	Shoots bullet from player to cursor position
	Deals damage to enemy if bullet collides with enemy position

  - isbullet_enemy_colliding
	Returns true if bullet is colliding with enemy, else returns false
*****************************************************************************************************/

//void weapon_fire(f32 player_x, f32 player_y, int state);
//bool isbullet_enemy_colliding(f32 bullet_x, f32 bullet_y, f32 enemy_x, f32 enemy_y);
//bool isbullet_enemy_colliding(f32 bullet_x, f32 bullet_y, f32 enemy_x, f32 enemy_y, f32 width, f32 height);


//#define BOSS_TIMER 0.5f
//
//struct Bullet {
//	f32 x{}, y{};
//	AEVec2 center{};
//	f32 width, height;
//	f32 speed;
//	f32 timer;
//	bool isTimerActive, isTeleported, isShooting;
//
//	// ----- Mesh & Texture -----
//	AEGfxTexture* bulletTex;
//};