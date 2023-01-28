#pragma once

#include "AEEngine.h"

extern AEVec2 EnemyCenter;

void enemy_init();
void draw_enemy();
AEVec2 enemy_update(AEVec2 EnemyCenter);
