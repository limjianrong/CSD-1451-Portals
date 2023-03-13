#pragma once

#include "AEEngine.h"
#include "Player.hpp"

#define ENEMY3_HEIGHT 60.f
#define ENEMY3_WIDTH 100.f

#define WARNING_HEIGHT 80.f
#define WARNING_WIDTH 80.f

struct Enemy3_stats {
	f32 x{}, y{}, rotation{ 0.0f };
	s32 Hp{ 15 };
	AEVec2 center{ 0,0 };
};

void enemy3_load();
void enemy3_init();
void enemy3_update(Player_stats* player);
void draw_enemy3();
void enemy3_free();
void enemy3_unload();

void enemy3_collision(Player_stats* player);



