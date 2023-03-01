#pragma once

#include "AEEngine.h"

#define CARD_WIDTH 200.f
#define CARD_HEIGHT 300.f

struct card {
	//coordinates
	f32 x, y;
};

void upgrades_init();
void draw_upgrade_cards();
void update_upgrade_cards();