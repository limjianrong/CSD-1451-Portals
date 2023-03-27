#pragma once

#include "AEEngine.h"

// ---- Upgrade Card ----
#define CARD_WIDTH 200.f			// Card width
#define CARD_HEIGHT 300.f			// Card height
#define CARD_ROTATION PI			// Card rotation
#define MAX_UPGRADES 4				// Change this to max amount of upgrades


static enum card_type {
	MAX_HP_card,
	MOVEMENT_SPEED_card,
	PORTAL_RANGE_card,
	SHIELD_card, 
	NO_card
};

struct Card {
	s32 type;
	f32 x, y;
	AEVec2 center;

	// ---- Draw ----
	AEMtx33 scale, rotate, translate, transform; // TRS
	AEGfxTexture* Texture;
};


struct Shield 
{
	f32 x, y, rotation;
	AEVec2 center;
	f32 width, height;

	// ---- Draw ----
	AEMtx33 scale, rotate, translate, transform; // TRS
	AEGfxTexture* Texture;
};

void drawing_mesh(Shield& shield, Player_stats& player);

void upgrades_load();
void upgrades_init();
void upgrade_draw();
void upgrade_update();
void upgrades_free();
void upgrades_unload();


void shield_upgrade_update();
void shield_upgrade_draw();
void render_card(s32 card, f32 transX, f32 offsetY);