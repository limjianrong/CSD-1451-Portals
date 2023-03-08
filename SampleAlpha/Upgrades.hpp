#pragma once

#include "AEEngine.h"

#define CARD_WIDTH 200.f
#define CARD_HEIGHT 300.f

static enum card_type {
	MAX_HP_card,
	MOVEMENT_SPEED_card,
	PORTAL_RANGE_card,
	NO_card
};

struct Card {
	s32 type;
	f32 x{}, y{}, rotation;
	AEVec2 center;
	f32 width{ CARD_WIDTH }, height{ CARD_HEIGHT };

	// ---- Draw ----
	AEMtx33 scale, rotate, translate, transform; // TRS
	AEGfxTexture* Texture;
};

void upgrades_load();
void upgrades_init();
void upgrade_draw();
void upgrade_update();


void update_upgrade_cards();