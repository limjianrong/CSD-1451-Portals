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

extern AEVec2 EnemyCenter;

void enemy_init();
void draw_enemy();
AEVec2 enemy_update(AEVec2 EnemyCenter);
