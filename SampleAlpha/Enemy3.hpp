/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Enemy3.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file declares the functions for Enemy3, struct Enemy3_stats, and 
  defines enemy3 & warning label scale.
==================================================================================*/
#pragma once

#include "AEEngine.h"

const float ENEMY3_HEIGHT{ 80.f };
const float ENEMY3_WIDTH{ 80.f };

const float ENEMY3_DROPPED_XP{ 30.f };

const float WARNING_HEIGHT{ 80.f };
const float WARNING_WIDTH{ 80.f };

struct Enemy3_stats : GameObjects {

};

void enemy3_load();
void enemy3_init();
void enemy3_update(Player_stats* player);
void draw_enemy3();
void enemy3_unload();

void enemy3_collision();



